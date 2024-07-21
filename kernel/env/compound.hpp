#pragma once

#include <cstdint>
#include <cstddef>
#include <stddef.h>
#include <cstdio>

#include "env/env.hpp"
#include "env/element.hpp"

#define MAX_compound_element 8
enum compound_TYPE{
    compound_TYPE_NONE,
    compound_TYPE_CODE,
    compound_TYPE_DATA,
};
struct compound{
    union{
        element_t elements[MAX_compound_element];
        bio_data_t data;
    }elem;
    int len;
    size_t size;
    char* fname;
    int free;
};
struct compound *compound_init(struct compound *comp);
void compound_print(struct compound *comp);
void compound_print_data(struct compound *comp);
void compound_code_push(struct compound *comp,element_t element);
void compound_data_push(struct compound *comp,bio_data_t data);
int compound_type(struct compound *comp);
void compound_bond(struct compound *comp,struct bio_env *be);
void compound_free(struct compound *comp);
#define compoundINIT compound_init(new struct compound)
