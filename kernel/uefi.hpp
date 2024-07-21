#pragma once


//#include <Uefi.h>

#define IN
#define OUT
#define OPTIONAL

#define VOID void
///
/// 8-byte unsigned value
///
typedef unsigned long long UINT64;
///
/// 8-byte signed value
///
typedef long long INT64;
///
/// 4-byte unsigned value
///
typedef unsigned int UINT32;
///
/// 4-byte signed value
///
typedef int INT32;
///
/// 2-byte unsigned value
///
typedef unsigned short UINT16;
///
/// 2-byte Character.  Unless otherwise specified all strings are stored in the
/// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
///
typedef unsigned short CHAR16;
///
/// 2-byte signed value
///
typedef short INT16;
///
/// Logical Boolean.  1-byte value containing 0 for FALSE or a 1 for TRUE.  Other
/// values are undefined.
///
typedef unsigned char BOOLEAN;
///
/// 1-byte unsigned value
///
typedef unsigned char UINT8;
///
/// 1-byte Character
///
typedef char CHAR8;
///
/// 1-byte signed value
///
typedef signed char INT8;


typedef UINT64 UINTN;
///
/// Signed value of native width.  (4 bytes on supported 32-bit processor instructions,
/// 8 bytes on supported 64-bit processor instructions)
///
typedef INT64 INTN;
#define RETURN_SUCCESS  0
#define EFI_SUCCESS               RETURN_SUCCESS

///
/// 128-bit buffer containing a unique identifier value.
///
typedef struct {
  UINT32    Data1;
  UINT16    Data2;
  UINT16    Data3;
  UINT8     Data4[8];
} GUID;
typedef GUID EFI_GUID;
///
/// Function return status for EFI API.
///
typedef UINTN RETURN_STATUS;
typedef RETURN_STATUS EFI_STATUS;
///
/// A collection of related interfaces.
///
typedef void *EFI_HANDLE;
///
/// Handle to an event structure.
///
typedef void *EFI_EVENT;
///
/// Task priority level.
///
typedef UINTN EFI_TPL;
///
/// Logical block address.
///
typedef UINT64 EFI_LBA;

///
/// 64-bit physical memory address.
///
typedef UINT64 EFI_PHYSICAL_ADDRESS;

///
/// 64-bit virtual memory address.
///
typedef UINT64 EFI_VIRTUAL_ADDRESS;
#define EFIAPI __attribute__((ms_abi))

typedef enum {
  ///
  /// Not used.
  ///
  EfiReservedMemoryType,
  ///
  /// The code portions of a loaded application.
  /// (Note that UEFI OS loaders are UEFI applications.)
  ///
  EfiLoaderCode,
  ///
  /// The data portions of a loaded application and the default data allocation
  /// type used by an application to allocate pool memory.
  ///
  EfiLoaderData,
  ///
  /// The code portions of a loaded Boot Services Driver.
  ///
  EfiBootServicesCode,
  ///
  /// The data portions of a loaded Boot Serves Driver, and the default data
  /// allocation type used by a Boot Services Driver to allocate pool memory.
  ///
  EfiBootServicesData,
  ///
  /// The code portions of a loaded Runtime Services Driver.
  ///
  EfiRuntimeServicesCode,
  ///
  /// The data portions of a loaded Runtime Services Driver and the default
  /// data allocation type used by a Runtime Services Driver to allocate pool memory.
  ///
  EfiRuntimeServicesData,
  ///
  /// Free (unallocated) memory.
  ///
  EfiConventionalMemory,
  ///
  /// Memory in which errors have been detected.
  ///
  EfiUnusableMemory,
  ///
  /// Memory that holds the ACPI tables.
  ///
  EfiACPIReclaimMemory,
  ///
  /// Address space reserved for use by the firmware.
  ///
  EfiACPIMemoryNVS,
  ///
  /// Used by system firmware to request that a memory-mapped IO region
  /// be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
  ///
  EfiMemoryMappedIO,
  ///
  /// System memory-mapped IO region that is used to translate memory
  /// cycles to IO cycles by the processor.
  ///
  EfiMemoryMappedIOPortSpace,
  ///
  /// Address space reserved by the firmware for code that is part of the processor.
  ///
  EfiPalCode,
  ///
  /// A memory region that operates as EfiConventionalMemory,
  /// however it happens to also support byte-addressable non-volatility.
  ///
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;
typedef struct {
  ///
  /// Type of the memory region.
  /// Type EFI_MEMORY_TYPE is defined in the
  /// AllocatePages() function description.
  ///
  UINT32                  Type;
  ///
  /// Physical address of the first byte in the memory region. PhysicalStart must be
  /// aligned on a 4 KiB boundary, and must not be above 0xfffffffffffff000. Type
  /// EFI_PHYSICAL_ADDRESS is defined in the AllocatePages() function description
  ///
  EFI_PHYSICAL_ADDRESS    PhysicalStart;
  ///
  /// Virtual address of the first byte in the memory region.
  /// VirtualStart must be aligned on a 4 KiB boundary,
  /// and must not be above 0xfffffffffffff000.
  ///
  EFI_VIRTUAL_ADDRESS     VirtualStart;
  ///
  /// NumberOfPagesNumber of 4 KiB pages in the memory region.
  /// NumberOfPages must not be 0, and must not be any value
  /// that would represent a memory page with a start address,
  /// either physical or virtual, above 0xfffffffffffff000.
  ///
  UINT64                  NumberOfPages;
  ///
  /// Attributes of the memory region that describe the bit mask of capabilities
  /// for that memory region, and not necessarily the current settings for that
  /// memory region.
  ///
  UINT64                  Attribute;
} EFI_MEMORY_DESCRIPTOR;
///
/// Enumeration of reset types.
///
typedef enum {
  ///
  /// Used to induce a system-wide reset. This sets all circuitry within the
  /// system to its initial state.  This type of reset is asynchronous to system
  /// operation and operates withgout regard to cycle boundaries.  EfiColdReset
  /// is tantamount to a system power cycle.
  ///
  EfiResetCold,
  ///
  /// Used to induce a system-wide initialization. The processors are set to their
  /// initial state, and pending cycles are not corrupted.  If the system does
  /// not support this reset type, then an EfiResetCold must be performed.
  ///
  EfiResetWarm,
  ///
  /// Used to induce an entry into a power state equivalent to the ACPI G2/S5 or G3
  /// state.  If the system does not support this reset type, then when the system
  /// is rebooted, it should exhibit the EfiResetCold attributes.
  ///
  EfiResetShutdown,
  ///
  /// Used to induce a system-wide reset. The exact type of the reset is defined by
  /// the EFI_GUID that follows the Null-terminated Unicode string passed into
  /// ResetData. If the platform does not recognize the EFI_GUID in ResetData the
  /// platform must pick a supported reset type to perform. The platform may
  /// optionally log the parameters from any non-normal reset that occurs.
  ///
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;

///
/// Data structure that precedes all of the standard EFI table types.
///
typedef struct {
  ///
  /// A 64-bit signature that identifies the type of table that follows.
  /// Unique signatures have been generated for the EFI System Table,
  /// the EFI Boot Services Table, and the EFI Runtime Services Table.
  ///
  UINT64    Signature;
  ///
  /// The revision of the EFI Specification to which this table
  /// conforms. The upper 16 bits of this field contain the major
  /// revision value, and the lower 16 bits contain the minor revision
  /// value. The minor revision values are limited to the range of 00..99.
  ///
  UINT32    Revision;
  ///
  /// The size, in bytes, of the entire table including the EFI_TABLE_HEADER.
  ///
  UINT32    HeaderSize;
  ///
  /// The 32-bit CRC for the entire table. This value is computed by
  /// setting this field to 0, and computing the 32-bit CRC for HeaderSize bytes.
  ///
  UINT32    CRC32;
  ///
  /// Reserved field that must be set to 0.
  ///
  UINT32    Reserved;
} EFI_TABLE_HEADER;
typedef struct {
  UINT16    Year;
  UINT8     Month;
  UINT8     Day;
  UINT8     Hour;
  UINT8     Minute;
  UINT8     Second;
  UINT8     Pad1;
  UINT32    Nanosecond;
  INT16     TimeZone;
  UINT8     Daylight;
  UINT8     Pad2;
} EFI_TIME;
typedef struct {
  ///
  /// Provides the reporting resolution of the real-time clock device in
  /// counts per second. For a normal PC-AT CMOS RTC device, this
  /// value would be 1 Hz, or 1, to indicate that the device only reports
  /// the time to the resolution of 1 second.
  ///
  UINT32     Resolution;
  ///
  /// Provides the timekeeping accuracy of the real-time clock in an
  /// error rate of 1E-6 parts per million. For a clock with an accuracy
  /// of 50 parts per million, the value in this field would be
  /// 50,000,000.
  ///
  UINT32     Accuracy;
  ///
  /// A TRUE indicates that a time set operation clears the device's
  /// time below the Resolution reporting level. A FALSE
  /// indicates that the state below the Resolution level of the
  /// device is not cleared when the time is set. Normal PC-AT CMOS
  /// RTC devices set this value to FALSE.
  ///
  BOOLEAN    SetsToZero;
} EFI_TIME_CAPABILITIES;
typedef struct {
  ///
  /// A GUID that defines the contents of a capsule.
  ///
  EFI_GUID    CapsuleGuid;
  ///
  /// The size of the capsule header. This may be larger than the size of
  /// the EFI_CAPSULE_HEADER since CapsuleGuid may imply
  /// extended header entries
  ///
  UINT32      HeaderSize;
  ///
  /// Bit-mapped list describing the capsule attributes. The Flag values
  /// of 0x0000 - 0xFFFF are defined by CapsuleGuid. Flag values
  /// of 0x10000 - 0xFFFFFFFF are defined by this specification
  ///
  UINT32      Flags;
  ///
  /// Size in bytes of the capsule.
  ///
  UINT32      CapsuleImageSize;
} EFI_CAPSULE_HEADER;
typedef EFIAPI EFI_STATUS(EFIAPI *EFI_GET_TIME)(OUT EFI_TIME *Time,OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL);
typedef enum {
  ///
  /// An event's timer settings is to be cancelled and not trigger time is to be set/
  ///
  TimerCancel,
  ///
  /// An event is to be signaled periodically at a specified interval from the current time.
  ///
  TimerPeriodic,
  ///
  /// An event is to be signaled once at a specified interval from the current time.
  ///
  TimerRelative
} EFI_TIMER_DELAY;
typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIMER)(
  IN  EFI_EVENT                Event,
  IN  EFI_TIMER_DELAY          Type,
  IN  UINT64                   TriggerTime
  );
typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIME)(
  IN  EFI_TIME                     *Time
  );
typedef
EFI_STATUS
(EFIAPI *EFI_GET_WAKEUP_TIME)(
  OUT BOOLEAN                     *Enabled,
  OUT BOOLEAN                     *Pending,
  OUT EFI_TIME                    *Time
  );
typedef
EFI_STATUS
(EFIAPI *EFI_SET_WAKEUP_TIME)(
  IN  BOOLEAN                      Enable,
  IN  EFI_TIME                     *Time   OPTIONAL
  );
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
  IN  UINTN                        MemoryMapSize,
  IN  UINTN                        DescriptorSize,
  IN  UINT32                       DescriptorVersion,
  IN  EFI_MEMORY_DESCRIPTOR        *VirtualMap
  );
typedef
EFI_STATUS
(EFIAPI *EFI_CONVERT_POINTER)(
  IN     UINTN                      DebugDisposition,
  IN OUT void                       **Address
  );
typedef
EFI_STATUS
(EFIAPI *EFI_GET_VARIABLE)(
  IN     CHAR16                      *VariableName,
  IN     EFI_GUID                    *VendorGuid,
  OUT    UINT32                      *Attributes     OPTIONAL,
  IN OUT UINTN                       *DataSize,
  OUT    void                        *Data           OPTIONAL
  );
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(
  IN OUT UINTN                    *VariableNameSize,
  IN OUT CHAR16                   *VariableName,
  IN OUT EFI_GUID                 *VendorGuid
  );
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VARIABLE)(
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid,
  IN  UINT32                       Attributes,
  IN  UINTN                        DataSize,
  IN  void                         *Data
  );
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(
  OUT UINT32                  *HighCount
  );
typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM)(
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  );
typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CAPSULE)(
  IN EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
  IN UINTN                  CapsuleCount,
  IN EFI_PHYSICAL_ADDRESS   ScatterGatherList   OPTIONAL
  );
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
  IN  EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
  IN  UINTN                  CapsuleCount,
  OUT UINT64                 *MaximumCapsuleSize,
  OUT EFI_RESET_TYPE         *ResetType
  );
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
  IN  UINT32            Attributes,
  OUT UINT64            *MaximumVariableStorageSize,
  OUT UINT64            *RemainingVariableStorageSize,
  OUT UINT64            *MaximumVariableSize
  );

typedef struct {
  ///
  /// The table header for the EFI Runtime Services Table.
  ///
  EFI_TABLE_HEADER Hdr;

  //
  // Time Services
  //
  EFI_GET_TIME GetTime;
  EFI_SET_TIME SetTime;
  EFI_GET_WAKEUP_TIME GetWakeupTime;
  EFI_SET_WAKEUP_TIME SetWakeupTime;

  //
  // Virtual Memory Services
  //
  EFI_SET_VIRTUAL_ADDRESS_MAP       SetVirtualAddressMap;
  EFI_CONVERT_POINTER               ConvertPointer;

  //
  // Variable Services
  //
  EFI_GET_VARIABLE                  GetVariable;
  EFI_GET_NEXT_VARIABLE_NAME        GetNextVariableName;
  EFI_SET_VARIABLE                  SetVariable;

  //
  // Miscellaneous Services
  //
  EFI_GET_NEXT_HIGH_MONO_COUNT      GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM                  ResetSystem;

  //
  // UEFI 2.0 Capsule Services
  //
  EFI_UPDATE_CAPSULE                UpdateCapsule;
  EFI_QUERY_CAPSULE_CAPABILITIES    QueryCapsuleCapabilities;

  //
  // Miscellaneous UEFI 2.0 Service
  //
  EFI_QUERY_VARIABLE_INFO           QueryVariableInfo;
} EFI_RUNTIME_SERVICES;



inline EFI_RUNTIME_SERVICES* uefi_rt;