#pragma once

#include "env/list.hpp"
#include "env/DNA.hpp"
#include "env/cellresult.hpp"

#define DEFAULT_lifespan 40

int cellbin_flag_GET();
void cellbin_flag_SET(int flag);

class cell{
private:
    int lifespan;
    int remaining_life;
    int fitness;
    void metabolism_and_motion();
    int grow();
    void proliferation();
    void death();
public:
    cell(int lifespan);
    ~cell();
    int run();
    void print();
    void arrangement();
    void enable();
    void disable();
    void cell_protein_push(struct protein *protein);
    int is_use;
    list *protein;
    struct DNA *DNA;
};