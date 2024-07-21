#pragma once

#include "env/list.hpp"

struct DNA{
    list *codn;
    list *protein;
};
void DNA_init(struct DNA *DNA);
void DNA_codn_push(struct DNA *DNA,struct codn *codn);
void DNA_print(struct DNA *DNA);
void DNA_setbone(struct DNA *DNA,list *list);
void DNA_setprot(struct DNA *DNA,list **list);
void DNA_free(struct DNA *DNA);