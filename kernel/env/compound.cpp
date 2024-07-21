#include "env/compound.hpp"

#include "env/cushionmodel.hpp"

struct compound *compound_init(struct compound *comp){
    comp->size=NULL;
    comp->len=0;
    comp->free=0;
    comp->fname=NULL;
    return comp;
}

void compound_code_push(struct compound *comp,element_t element){
    if(comp->len > MAX_compound_element)return;
    comp->elem.elements[comp->len]=element;
    comp->len++;
    comp->size=sizeof(element_t);
}

void compound_data_push(struct compound *comp,bio_data_t data){
    comp->elem.data=data;
    comp->size=sizeof(bio_data_t);
}

void compound_print(struct compound *comp){
    if(comp==NULL)return;
    EC.log(LWarn,"<");
    for(int i=0;i<comp->len;i++){
        EC.log(LWarn,"%02x",comp->elem.elements[i]);
        if(i!=comp->len-1) EC.log(LWarn," ");
    }
    EC.log(LWarn,">");
}

void compound_print_data(struct compound *comp){
    EC.log(LWarn,"[%010x]",comp->elem.data);
}

int compound_type(struct compound *comp){
    if(comp->size==sizeof(bio_data_t)) return compound_TYPE_DATA;
    return compound_TYPE_CODE;
}

void compound_bond(struct compound *comp,struct bio_env *be){
    for(int i=0;i<comp->len;i++){
       be->codedata_bond[i+be->codedata_bond_len]=comp->elem.elements[i];
    }
    be->codedata_bond_len+=comp->len;
}

void compound_free(struct compound *comp){
    if(comp!=NULL && comp->free!=1 && comp->size!=NULL){
        delete comp;
        comp->free=1;
    }
}
