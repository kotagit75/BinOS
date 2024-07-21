#pragma once

#include "env/compound.hpp"

struct codn{
    struct compound *compound;
    int data[MAX_compound_element];
};
void codn_init(struct codn *codn);
void codn_print(struct codn *codn);
void codn_print_compound(struct codn *codn);
void codn_free(struct codn *codn);
int codn_setcomp(struct codn *codn,struct compound *comp);