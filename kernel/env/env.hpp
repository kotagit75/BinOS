#pragma once

#include <vector>

#include "env/cell.hpp"
#include "env/eval.hpp"
#include "env/cushionmodel.hpp"

#define CELL_MAX 100
typedef unsigned long long bio_data_t;

struct bio_env{
    list *compound;
    std::vector<cell*> cell={};
    int codedata_bond[100];
    int codedata_bond_len;
    int cycle;
    int cycleMAX;
};

void InitializeBio_Env(int cycleMAX);
void bio_env_main(Ecushion ec,int cycleMAX);
void bio_env_run();
void bio_env_runcycle();
void bio_env_print();
void bio_env_cell_push(cell *cell);
void bio_env_compound_push(struct compound *comp);
void bio_env_compound_pop(struct compound *comp);
struct compound *bio_env_compound_pop_FILTER(int filter);
struct compound *bio_env_compound_pop_FILTER_random(int filter);
void bio_env_finish();
void bio_env_success(cell *cell);
bio_env *GetBE();

