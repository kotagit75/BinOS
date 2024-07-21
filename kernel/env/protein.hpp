#pragma once

#include "env/list.hpp"

struct protein{
    list *compound;
};
struct protein *protein_init(struct protein *protein);
void protein_compound_push(struct protein *protein,struct compound *comp);
void protein_bond(struct protein *protein,struct bio_env *be);
void protein_print(struct protein *protein);
void protein_free(struct protein *protein);
#define proteinINIT protein_init(new struct protein)