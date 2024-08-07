#include "printk.h"

#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "console.hpp"

extern "C" int printk(const char* format, ...) {
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}