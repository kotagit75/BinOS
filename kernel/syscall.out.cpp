/* Copyright (C) 1991-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/*#include "syscall.hpp"

#include <array>
#include <cstdint>

#include "interrupt.hpp"
#include "asmfunc.h"
#include "logger.hpp"
#include "NULL.hpp"
#include "segment.hpp"*/
SyscallLog *log=NULL;
namespace syscall {
Result LogChar( uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6,int Logindex=Logindex_count+1) {
    Log(kWarn,"%c,Logindex:%d",arg1,Logindex);
    log->SyscallLog_push((struct SYSCALLLOG_Type){Logindex,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
Result Hello( uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6,int Logindex=Logindex_count+1) {
    Log(kWarn,"Hello\n");
    log->SyscallLog_push((struct SYSCALLLOG_Type){Logindex,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
Result LogChar6( uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6,int Logindex=Logindex_count+1) {
    Log(kWarn,"%c%c%c%c%c%c",arg1,arg2,arg3,arg4,arg5,arg6);
    log->SyscallLog_push((struct SYSCALLLOG_Type){Logindex,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
Result None( uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6,int Logindex=Logindex_count+1) {
    return Result{0,0};
}
}
extern "C" std::array<SyscallFuncType*,0x11> syscall_table={
  (SyscallFuncType*)syscall::LogChar,
  (SyscallFuncType*)syscall::Hello,
  (SyscallFuncType*)syscall::LogChar6,
  (SyscallFuncType*)syscall::None,
};
extern "C" SyscallFuncType* syscall_table_index_GET(int index){
  return index>(4 -1)?(SyscallFuncType*)syscall::None:syscall_table_GET()[index];
}
std::array<SyscallFuncType*,0x11> syscall_table_GET(){
  return syscall_table;
}
SyscallLog::SyscallLog(){
}
SyscallLog::~SyscallLog(){
}
struct SYSCALLLOG_Type SyscallLog::SyscallLog_pop(){
  if(SyscallLog_size()==0)Log(kWarn,"SyscallLog.size()==0");
  struct SYSCALLLOG_Type Log = slog.front();
  slog.erase(slog.begin());
  return Log;
}
void SyscallLog::SyscallLog_push(struct SYSCALLLOG_Type type){
  slog.push_back(type);
}
int SyscallLog::SyscallLog_size(){
  return slog.size();
}
void SyscallLog::SyscallLog_clear(){
  this->slog.clear();
}
SyscallLog *SlogGet(){
  return log;
}
void InitializeSyscallLog(){
  if(log!=NULL)delete log;
  log=new SyscallLog;
}
void InitializeSyscall(){
  InitializeSyscallLog();
}
