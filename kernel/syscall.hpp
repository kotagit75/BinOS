#pragma once

#include <array>
#include <vector>

#include <cstdint>

struct Result {
    uint64_t value;
    int error;
};


using SyscallFuncType = Result(*)(uint64_t, uint64_t, uint64_t,
                                         uint64_t, uint64_t, uint64_t,int);

std::array<SyscallFuncType*,0x11> syscall_table_GET();
extern "C" SyscallFuncType* syscall_table_index_GET(int index);

struct SYSCALLLOG_Type{int syscallNum;uint64_t arg1;uint64_t arg2;uint64_t arg3;uint64_t arg4;uint64_t arg5;uint64_t arg6;};

class SyscallLog
{
private:
    std::vector<struct SYSCALLLOG_Type> slog;
public:
    SyscallLog();
    ~SyscallLog();
    struct SYSCALLLOG_Type SyscallLog_pop();
    void SyscallLog_push(struct SYSCALLLOG_Type type);
    int SyscallLog_size();
    void SyscallLog_clear();
};

SyscallLog *SlogGet();
void InitializeSyscallLog();
void InitializeSyscall();