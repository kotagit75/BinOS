#pragma once

#include "env/eval.hpp"
#include "env/cushionmodel.hpp"

class myrunEval:public runEval
{
private:
public:
    myrunEval();
    ~myrunEval();
    int run(char buf[100]);
    int Eval(char buf[100],cell_result result);
};

int EC_log(ELogLevel level,const char *format,...);

void EC_success(cell *cell);
void EC_finish();