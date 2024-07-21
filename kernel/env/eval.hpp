#pragma once

#include "env/cell.hpp"

class runEval
{
private:
public:
    runEval(){};
    ~runEval(){};
    virtual int run(char buf[100])=0;
    virtual int Eval(char buf[100],cell_result result)=0;
};
