#include "syscall.hpp"

#include <array>
#include <cstdint>

#include "interrupt.hpp"
#include "asmfunc.h"
#include "logger.hpp"
#include "NULL.hpp"
#include "segment.hpp"

SyscallLog *log=NULL;


namespace syscall {
#define SYSCALL(name) \
  Result name( \
      uint64_t arg1, uint64_t arg2, uint64_t arg3, \
      uint64_t arg4, uint64_t arg5, uint64_t arg6)

SYSCALL(LogChar) {
    Log(kWarn,"%c",arg1);
    log->SyscallLog_push((struct SYSCALLLOG_Type){0,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
SYSCALL(Hello) {
    Log(kWarn,"Hello\n");
    log->SyscallLog_push((struct SYSCALLLOG_Type){1,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
SYSCALL(LogChar6) {
    Log(kWarn,"%c%c%c%c%c%c",arg1,arg2,arg3,arg4,arg5,arg6);
    log->SyscallLog_push((struct SYSCALLLOG_Type){2,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
SYSCALL(None) {
    log->SyscallLog_push((struct SYSCALLLOG_Type){3,arg1,arg2,arg3,arg4,arg5,arg6});
    return Result{0,0};
}
#undef SYSCALL

}

#define SYSCALL_COUNT 4

extern "C" std::array<SyscallFuncType*,0x11> syscall_table={
  (SyscallFuncType*)syscall::LogChar,
  (SyscallFuncType*)syscall::Hello,
  (SyscallFuncType*)syscall::LogChar6,
  (SyscallFuncType*)syscall::None,
};

extern "C" SyscallFuncType* syscall_table_index_GET(int index){
  return index<0||index>(SYSCALL_COUNT-1)?(SyscallFuncType*)syscall::None:syscall_table_GET()[index];
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