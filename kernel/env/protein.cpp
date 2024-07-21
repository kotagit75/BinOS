#include "env/protein.hpp"

#include "env/cushionmodel.hpp"
#include "env/compound.hpp"

struct protein *protein_init(struct protein *protein){
    protein->compound=new list;
    list_init(protein->compound);
    return protein;
}

void protein_compound_push(struct protein *protein,struct compound *comp){
    list_insertion(protein->compound,comp);
}

void protein_print(struct protein *protein){
    if(protein==NULL)return;
    node *ptr=protein->compound->head;
    while(ptr!=NULL){
        compound_print(((struct compound*)(ptr->data)));
        if(ptr->next!=NULL)EC.log(LWarn,"-");
        ptr=ptr->next;
    }
}

void protein_bond(struct protein *protein,struct bio_env *be){
    node *ptr=protein->compound->head;
    while(ptr!=NULL){
        compound_bond(((struct compound*)(ptr->data)),be);    
        ptr=ptr->next;
    }
}

void protein_free(struct protein *protein){
    if(protein==NULL)return;
    if(protein->compound!=NULL){
        node *ptr=protein->compound->head;
        while(ptr!=NULL){
            compound_free(((struct compound*)(ptr->data)));
            ptr=ptr->next;
        }
    }
    delete protein->compound;
    delete protein;
}