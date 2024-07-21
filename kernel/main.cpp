/**
 * @file main.cpp
 *
 * カーネル本体のプログラムを書いたファイル．
 */

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string.h>
#include <deque>
#include <limits>
#include <numeric>
#include <vector>

#include "frame_buffer_config.hpp"
#include "memory_map.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"
#include "logger.hpp"
#include "interrupt.hpp"
#include "asmfunc.h"
#include "segment.hpp"
#include "paging.hpp"
#include "memory_manager.hpp"
#include "window.hpp"
#include "layer.hpp"
#include "message.hpp"
#include "timer.hpp"
#include "acpi.hpp"
#include "task.hpp"
#include "syscall.hpp"
#include "Logo.hpp"
#include "uefi.hpp"
#include "env/cell.hpp"
#include "env/env.hpp"
#include "env/cushionmodel.hpp"
#include "env/cushion/cushion.hpp"

int printk(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

extern "C" void KernelMainNewStack(
    const FrameBufferConfig& frame_buffer_config_ref,
    const MemoryMap& memory_map_ref,
    const acpi::RSDP& acpi_table,
    int cycleMAX,
    EFI_RUNTIME_SERVICES* rt) {
  MemoryMap memory_map{memory_map_ref};
  uefi_rt=rt;
  InitializeGraphics(frame_buffer_config_ref);
  InitializeConsole();
  SetLogLevel(kWarn);
  InitializeSegmentation();
  InitializePaging();
  InitializeMemoryManager(memory_map);
  InitializeInterrupt();
  InitializeLayer();
  layer_manager->Draw({{0, 0}, ScreenSize()});
  acpi::Initialize(acpi_table);
  InitializeLAPICTimer();
  InitializeTask();
  Task& main_task = task_manager->CurrentTask();
  InitializeSyscall();
  Ecushion ec={EC_log,LWarn,EC_success,EC_finish,(runEval*)new myrunEval};
  bio_env_main(ec,cycleMAX);
  while (1);
}

extern "C" void __cxa_pure_virtual() {
  while (1) __asm__("hlt");
}
