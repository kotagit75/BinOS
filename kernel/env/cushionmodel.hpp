#pragma once

#include <cstddef>
#include <cstdio>

#include "env/cell.hpp"
#include "env/eval.hpp"

enum ELogLevel{
  LError = 3,
  LWarn  = 4,
  LInfo  = 6,
  LDebug = 7,
};

typedef struct{
    int(*log)(ELogLevel,const char*,...);
    ELogLevel loglevel;
    void(*success)(cell*);
    void(*finish)();
    runEval *eval;
} Ecushion;

extern Ecushion EC;

#define ECLOG_debug(level,fmt,...) EC.log(level,"%s:%d " fmt,__func__,__LINE__,__VA_ARGS__)
#define ECLOG_debug_noargs(level,fmt) EC.log(level,"%s:%d " fmt,__func__,__LINE__)

void InitializeEC(Ecushion cushion);