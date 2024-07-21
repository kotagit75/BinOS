#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>
#include  <Library/UefiRuntimeServicesTableLib.h>
#include  <Library/PrintLib.h>
#include  <Library/MemoryAllocationLib.h>
#include  <Library/BaseMemoryLib.h>
#include  <Protocol/LoadedImage.h>
#include  <Protocol/SimpleFileSystem.h>
#include  <Protocol/DiskIo2.h>
#include  <Protocol/BlockIo.h>
#include  <Guid/FileInfo.h>
#include  "frame_buffer_config.hpp"
#include  "memory_map.hpp"
#include  "elf.hpp"
#include  <wchar.h>
#include  <string.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>

EFI_STATUS GetMemoryMap(struct MemoryMap* map) {
  if (map->buffer == NULL) {
    return EFI_BUFFER_TOO_SMALL;
  }

  map->map_size = map->buffer_size;
  return gBS->GetMemoryMap(
      &map->map_size,
      (EFI_MEMORY_DESCRIPTOR*)map->buffer,
      &map->map_key,
      &map->descriptor_size,
      &map->descriptor_version);
}

const CHAR16* GetMemoryTypeUnicode(EFI_MEMORY_TYPE type) {
  switch (type) {
    case EfiReservedMemoryType: return L"EfiReservedMemoryType";
    case EfiLoaderCode: return L"EfiLoaderCode";
    case EfiLoaderData: return L"EfiLoaderData";
    case EfiBootServicesCode: return L"EfiBootServicesCode";
    case EfiBootServicesData: return L"EfiBootServicesData";
    case EfiRuntimeServicesCode: return L"EfiRuntimeServicesCode";
    case EfiRuntimeServicesData: return L"EfiRuntimeServicesData";
    case EfiConventionalMemory: return L"EfiConventionalMemory";
    case EfiUnusableMemory: return L"EfiUnusableMemory";
    case EfiACPIReclaimMemory: return L"EfiACPIReclaimMemory";
    case EfiACPIMemoryNVS: return L"EfiACPIMemoryNVS";
    case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
    case EfiMemoryMappedIOPortSpace: return L"EfiMemoryMappedIOPortSpace";
    case EfiPalCode: return L"EfiPalCode";
    case EfiPersistentMemory: return L"EfiPersistentMemory";
    case EfiMaxMemoryType: return L"EfiMaxMemoryType";
    default: return L"InvalidMemoryType";
  }
}

EFI_STATUS SaveMemoryMap(struct MemoryMap* map, EFI_FILE_PROTOCOL* file) {
  EFI_STATUS status;
  CHAR8 buf[256];
  UINTN len;

  CHAR8* header =
    "Index, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
  len = AsciiStrLen(header);
  status = file->Write(file, &len, header);
  if (EFI_ERROR(status)) {
    return status;
  }

  Print(L"map->buffer = %08lx, map->map_size = %08lx\n",
      map->buffer, map->map_size);

  EFI_PHYSICAL_ADDRESS iter;
  int i;
  for (iter = (EFI_PHYSICAL_ADDRESS)map->buffer, i = 0;
       iter < (EFI_PHYSICAL_ADDRESS)map->buffer + map->map_size;
       iter += map->descriptor_size, i++) {
    EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)iter;
    len = AsciiSPrint(
        buf, sizeof(buf),
        "%u, %x, %-ls, %08lx, %lx, %lx\n",
        i, desc->Type, GetMemoryTypeUnicode(desc->Type),
        desc->PhysicalStart, desc->NumberOfPages,
        desc->Attribute & 0xffffflu);
    status = file->Write(file, &len, buf);
    if (EFI_ERROR(status)) {
      return status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL** root) {
  EFI_STATUS status;
  EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;

  status = gBS->OpenProtocol(
      image_handle,
      &gEfiLoadedImageProtocolGuid,
      (VOID**)&loaded_image,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if (EFI_ERROR(status)) {
    return status;
  }

  status = gBS->OpenProtocol(
      loaded_image->DeviceHandle,
      &gEfiSimpleFileSystemProtocolGuid,
      (VOID**)&fs,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if (EFI_ERROR(status)) {
    return status;
  }

  return fs->OpenVolume(fs, root);
}

EFI_STATUS OpenGOP(EFI_HANDLE image_handle,
                   EFI_GRAPHICS_OUTPUT_PROTOCOL** gop) {
  EFI_STATUS status;
  UINTN num_gop_handles = 0;
  EFI_HANDLE* gop_handles = NULL;

  status = gBS->LocateHandleBuffer(
      ByProtocol,
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      &num_gop_handles,
      &gop_handles);
  if (EFI_ERROR(status)) {
    return status;
  }

  status = gBS->OpenProtocol(
      gop_handles[0],
      &gEfiGraphicsOutputProtocolGuid,
      (VOID**)gop,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if (EFI_ERROR(status)) {
    return status;
  }

  FreePool(gop_handles);

  return EFI_SUCCESS;
}

const CHAR16* GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT fmt) {
  switch (fmt) {
    case PixelRedGreenBlueReserved8BitPerColor:
      return L"PixelRedGreenBlueReserved8BitPerColor";
    case PixelBlueGreenRedReserved8BitPerColor:
      return L"PixelBlueGreenRedReserved8BitPerColor";
    case PixelBitMask:
      return L"PixelBitMask";
    case PixelBltOnly:
      return L"PixelBltOnly";
    case PixelFormatMax:
      return L"PixelFormatMax";
    default:
      return L"InvalidPixelFormat";
  }
}

void Halt(void) {
  while (1) __asm__("hlt");
}

void CalcLoadAddressRange(Elf64_Ehdr* ehdr, UINT64* first, UINT64* last) {
  Elf64_Phdr* phdr = (Elf64_Phdr*)((UINT64)ehdr + ehdr->e_phoff);
  *first = MAX_UINT64;
  *last = 0;
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD) continue;
    *first = MIN(*first, phdr[i].p_vaddr);
    *last = MAX(*last, phdr[i].p_vaddr + phdr[i].p_memsz);
  }
}

void CopyLoadSegments(Elf64_Ehdr* ehdr) {
  Elf64_Phdr* phdr = (Elf64_Phdr*)((UINT64)ehdr + ehdr->e_phoff);
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD) continue;

    UINT64 segm_in_file = (UINT64)ehdr + phdr[i].p_offset;
    CopyMem((VOID*)phdr[i].p_vaddr, (VOID*)segm_in_file, phdr[i].p_filesz);

    UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz;
    SetMem((VOID*)(phdr[i].p_vaddr + phdr[i].p_filesz), remain_bytes, 0);
  }
}
EFI_FILE_PROTOCOL* root_dir;
EFI_STATUS SaveSuccess(CHAR8* code) {
  EFI_FILE_PROTOCOL* file;
  EFI_STATUS status;
  status = root_dir->Open(
      root_dir, &file, L"\\run",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\run': %r\n", status);
    Print(L"Ignored.\n");
  } else {    
    UINTN len=100;
    status = file->Write(file, &len, code);
    if (EFI_ERROR(status)) {
      Print(L"failed to save run: %r\n", status);
      Halt();
    }
    status = file->Close(file);
    if (EFI_ERROR(status)) {
      Print(L"failed to close run: %r\n", status);
      Halt();
    }
  }
  return EFI_SUCCESS;
}

int myatoi(unsigned short *str) {
    int num = 0;
    int i=1;
    int j=0;
    while(str[j] != L'\0')j++;
    j--;
    for(int k=0;k<j;k++)i*=10;
    j=0;
    if(i==1 && str[0]==L'9'){
      num=9;
    }
    else{
      while(str[j] != L'\0'){
        if(str[j]==L'1'){
          num+=1*i;
        }else if(str[j]==L'2'){
          num+=2*i;
        }else if(str[j]==L'3'){
          num+=3*i;
        }else if(str[j]==L'4'){
          num+=4*i;
        }else if(str[j]==L'5'){
          num+=5*i;
        }else if(str[j]==L'6'){
          num+=6*i;
        }else if(str[j]==L'7'){
          num+=7*i;
        }else if(str[j]==L'8'){
          num+=8*i;
        }else if(str[j]==L'9'){
          num+=9*i;
        }
        i/=10;
        j++;
      }
    }
    return num;
}

VOID Rt_ResetSystem(
  EFI_RESET_TYPE           ResetType,
  EFI_STATUS               ResetStatus,
  UINTN                    DataSize,
  VOID                     *ResetData){
    gRT->ResetSystem(ResetType,ResetStatus,DataSize,ResetData);
}

struct RT_co{
  VOID(*ResetSystem)(IN EFI_RESET_TYPE ResetType,  IN EFI_STATUS ResetStatus,  IN UINTN DataSize,  IN VOID *ResetData OPTIONAL  );
};


EFI_STATUS EFIAPI UefiMain(
    EFI_HANDLE image_handle,
    EFI_SYSTEM_TABLE* system_table) {
  EFI_STATUS status;
  Print(L"Hello, Bi World!\n");


#define cycleMAX_strMAX 4
#define cycleMAX_default 100

  Print(L"Please enter the number of cycles(enter->%d):",cycleMAX_default);

  EFI_INPUT_KEY key;
  unsigned short cycleMAX_str[cycleMAX_strMAX+1];
  int cycleMAX_str_count=0;
  int cycleMAX_p=cycleMAX_default;
  while (1){
    if (!system_table->ConIn->ReadKeyStroke(system_table->ConIn,&key)){
      if(key.UnicodeChar==L'\r'){
        break;
      }
      if(key.UnicodeChar==L'\b'){
        if(cycleMAX_str_count==0)continue;
        cycleMAX_str_count--;
        cycleMAX_str[cycleMAX_str_count]=0;
        Print(L"\nPlease enter the number of cycles(enter->%d):%s",cycleMAX_default,cycleMAX_str);
        continue;
      }
      cycleMAX_str[cycleMAX_str_count]=key.UnicodeChar;
      Print(L"%c",key.UnicodeChar);
      cycleMAX_str_count++;
      if(cycleMAX_str_count==cycleMAX_strMAX){
        break;
      }
    }
  }
  if(cycleMAX_str_count!=0){
    cycleMAX_str[cycleMAX_str_count]=0;
    cycleMAX_p=myatoi(cycleMAX_str);
  }
  const int cycleMAX=cycleMAX_p;

  Print(L"\n");

  //192.168.22.22
  //int ip[ip_MAX];

  /*EFI_INPUT_KEY key;
  int ip_i=0;
  unsigned short str[3];
  for(int j=0;j<ip_MAX;j++){
    unsigned short ip_stack[15];
    int i=0;
    while(1){
      if (!system_table->ConIn->ReadKeyStroke(system_table->ConIn, &key) && ip_i<ip_MAX) {
        if (key.UnicodeChar != L'\r') {
          str[0] = key.UnicodeChar;
          str[1] = L'\0';
          system_table->ConOut->OutputString(system_table->ConOut,
                                          str);
          if(key.UnicodeChar == L'.'){
            ip[ip_i]=myatoi(ip_stack);
            ip_i++;
            break;
          }else{
            ip_stack[i]=key.UnicodeChar;
            ip_stack[i+1] = L'\0';
            i++;
          }
        } else {
          ip[ip_i]=myatoi(ip_stack);
          break;
        }
      }
    }
    if(key.UnicodeChar == L'\r' || ip_i>=ip_MAX)break;
  }
  Print(L"\n");
  Print(L"ip:%d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);*/

  CHAR8 memmap_buf[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buf), memmap_buf, 0, 0, 0, 0};
  status = GetMemoryMap(&memmap);
  if (EFI_ERROR(status)) {
    Print(L"failed to get memory map: %r\n", status);
    Halt();
  }

  status = OpenRootDir(image_handle, &root_dir);
  if (EFI_ERROR(status)) {
    Print(L"failed to open root directory: %r\n", status);
    Halt();
  }

  EFI_FILE_PROTOCOL* memmap_file;
  status = root_dir->Open(
      root_dir, &memmap_file, L"\\memmap",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\memmap': %r\n", status);
    Print(L"Ignored.\n");
  } else {
    status = SaveMemoryMap(&memmap, memmap_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to save memory map: %r\n", status);
      Halt();
    }
    status = memmap_file->Close(memmap_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to close memory map: %r\n", status);
      Halt();
    }
  }

  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
  status = OpenGOP(image_handle, &gop);
  if (EFI_ERROR(status)) {
    Print(L"failed to open GOP: %r\n", status);
    Halt();
  }

  Print(L"Resolution: %ux%u, Pixel Format: %s, %u pixels/line\n",
      gop->Mode->Info->HorizontalResolution,
      gop->Mode->Info->VerticalResolution,
      GetPixelFormatUnicode(gop->Mode->Info->PixelFormat),
      gop->Mode->Info->PixelsPerScanLine);
  Print(L"Frame Buffer: 0x%0lx - 0x%0lx, Size: %lu bytes\n",
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize,
      gop->Mode->FrameBufferSize);

  //UINT8* frame_buffer = (UINT8*)gop->Mode->FrameBufferBase;
  /*for (UINTN i = 0; i < gop->Mode->FrameBufferSize; ++i) {
    frame_buffer[i] = 255;
  }*/
  struct RT_co *rtco;
  gBS->AllocatePool(EfiLoaderData, sizeof(struct RT_co *), (void**)&rtco);
  rtco->ResetSystem=Rt_ResetSystem;

  EFI_FILE_PROTOCOL* kernel_file;
  status = root_dir->Open(
      root_dir, &kernel_file, L"\\kernel.elf",
      EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\kernel.elf': %r\n", status);
    Halt();
  }

  UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
  UINT8 file_info_buffer[file_info_size];
  status = kernel_file->GetInfo(
      kernel_file, &gEfiFileInfoGuid,
      &file_info_size, file_info_buffer);
  if (EFI_ERROR(status)) {
    Print(L"failed to get file information: %r\n", status);
    Halt();
  }

  EFI_FILE_INFO* file_info = (EFI_FILE_INFO*)file_info_buffer;
  UINTN kernel_file_size = file_info->FileSize;

  VOID* kernel_buffer;
  status = gBS->AllocatePool(EfiLoaderData, kernel_file_size, &kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"failed to allocate pool: %r\n", status);
    Halt();
  }
  status = kernel_file->Read(kernel_file, &kernel_file_size, kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"error: %r", status);
    Halt();
  }

  Elf64_Ehdr* kernel_ehdr = (Elf64_Ehdr*)kernel_buffer;
  UINT64 kernel_first_addr, kernel_last_addr;
  CalcLoadAddressRange(kernel_ehdr, &kernel_first_addr, &kernel_last_addr);

  UINTN num_pages = (kernel_last_addr - kernel_first_addr + 0xfff) / 0x1000;
  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData,
                              num_pages, &kernel_first_addr);
  if (EFI_ERROR(status)) {
    Print(L"failed to allocate pages: %r\n", status);
    Halt();
  }

  CopyLoadSegments(kernel_ehdr);
  Print(L"Kernel: 0x%0lx - 0x%0lx\n", kernel_first_addr, kernel_last_addr);

  status = gBS->FreePool(kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"failed to free pool: %r\n", status);
    Halt();
  }

  status = gBS->ExitBootServices(image_handle, memmap.map_key);

  if (EFI_ERROR(status)) {
    status = GetMemoryMap(&memmap);
    if (EFI_ERROR(status)) {
      Print(L"failed to get memory map: %r\n", status);
      Halt();
    }
    status = gBS->ExitBootServices(image_handle, memmap.map_key);
    if (EFI_ERROR(status)) {
      Print(L"Could not exit boot service: %r\n", status);
      Halt();
    }
  }

  UINT64 entry_addr = *(UINT64*)(kernel_first_addr + 24);

  struct FrameBufferConfig config = {
    (UINT8*)gop->Mode->FrameBufferBase,
    gop->Mode->Info->PixelsPerScanLine,
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    0
  };
  switch (gop->Mode->Info->PixelFormat) {
    case PixelRedGreenBlueReserved8BitPerColor:
      config.pixel_format = kPixelRGBResv8BitPerColor;
      break;
    case PixelBlueGreenRedReserved8BitPerColor:
      config.pixel_format = kPixelBGRResv8BitPerColor;
      break;
    default:
      Print(L"Unimplemented pixel format: %d\n", gop->Mode->Info->PixelFormat);
      Halt();
  }

  VOID* acpi_table = NULL;
  for (UINTN i = 0; i < system_table->NumberOfTableEntries; ++i) {
    if (CompareGuid(&gEfiAcpiTableGuid,
                    &system_table->ConfigurationTable[i].VendorGuid)) {
      acpi_table = system_table->ConfigurationTable[i].VendorTable;
      break;
    }
  }



  //rtco->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0L);

  typedef void EntryPointType(const struct FrameBufferConfig*,
                              const struct MemoryMap*,
                              const VOID*,
                              const int,
                              EFI_RUNTIME_SERVICES*);
  EntryPointType* entry_point = (EntryPointType*)entry_addr;
  entry_point(&config, &memmap, acpi_table,cycleMAX,gRT);
  //gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, 0L);
  Print(L"All done\n");

  while (1);
  return EFI_SUCCESS;
}