#include "env/cushion/cushion.hpp"

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string.h>
#include <vector>

#include "env/eval.hpp"
#include "env/cell.hpp"
#include "env/cushionmodel.hpp"
#include "asmfunc.h"
#include "task.hpp"

#define Nomal_Fitness 50

namespace{
    int maintaskid;
}
myrunEval::myrunEval(){
    maintaskid=task_manager->CurrentTask().ID();
}
myrunEval::~myrunEval(){
}
char binbuf[100];
typedef void(*binFunc)(void(*)(void));
void task_cellbin_func(uint64_t task_id,int64_t data){
    ((binFunc)(binbuf))(SyscallEntry);
    EC.log(LInfo,"main_task_id:%d(subside)\n",maintaskid);
    EC.log(LInfo,"In task_cellbin_func,task_manager->SendMessage:%s\n",task_manager->SendMessage(maintaskid,Message{Message::kCellfinish}).Name());
    task_manager->CurrentTask().Sleep();
    while (1);
}
int cellbin_flag=1;
int cellbin_flag_GET(){
    return cellbin_flag;
}
void cellbin_flag_SET(int flag){
    EC.log(LInfo,"cellbin_flag_SET:flag=%d\n",flag);
    cellbin_flag=flag;
}
Task *cellbin_task=NULL;
int myrunEval::run(char buf[100]){
    for(int i=0;i<100;i++)binbuf[i]=buf[i];
    cellbin_flag_SET(1);
    int cellbin_Eflag=0;
    if(!cellbin_task){
        cellbin_task=&(task_manager->NewTask());
    }
    cellbin_task->InitContext(task_cellbin_func,0).Wakeup();
    while (cellbin_flag_GET()){
        __asm__("cli");
        auto msg = task_manager->CurrentTask().ReceiveMessage();
        if (!msg) {
            task_manager->CurrentTask().Sleep();
            __asm__("sti");
            continue;
        }
        __asm__("sti");
        switch(msg->type){
            case Message::kCellfinish:
                cellbin_flag_SET(0);
                break;
            case Message::kCellEfinish:
                cellbin_flag_SET(0);
                cellbin_Eflag=1;
                break;
            default:
                break;
        }
    }
    if(cellbin_Eflag){
        cellbin_task->Sleep();
    }
    return cellbin_Eflag;
}
int myrunEval::Eval(char buf[100],cell_result result){
    int count=0;
    if(result.Error){
      SlogGet()->SyscallLog_clear();
      InitializeSyscallLog();
      return Nomal_Fitness;
    }if(SlogGet()->SyscallLog_size()==0){
      SlogGet()->SyscallLog_clear();
      InitializeSyscallLog();
      return Nomal_Fitness;
    }
    struct SYSCALLLOG_Type log=SlogGet()->SyscallLog_pop();
    if(log.syscallNum==0){
        count++;
    }
    if(log.arg1=='K'){
        count++;
    }
    SlogGet()->SyscallLog_clear();
    InitializeSyscallLog();
    if(count==2){
        return 100;
    }
    if(count==1){
        return 80;
    }
    return 70;
}