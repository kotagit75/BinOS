#include "env/DNA.hpp"

#include "env/cushionmodel.hpp"
#include "env/protein.hpp"
#include "env/codn.hpp"

void DNA_init(struct DNA *DNA){
    DNA->codn=new list;
    list_init(DNA->codn);
    DNA->protein=new list;
    list_init(DNA->protein);
}

void DNA_codn_push(struct DNA *DNA,struct codn *codn){
    list_insertion(DNA->codn,codn);
}

void DNA_print(struct DNA *DNA){
    node *ptr=DNA->codn->head;
    while(ptr!=NULL){
        codn_print(((struct codn*)(ptr->data)));
        if(((struct codn*)(ptr->data))->compound!=NULL)EC.log(LWarn,"->");
        codn_print_compound(((struct codn*)(ptr->data)));
        if(ptr->next!=NULL)EC.log(LWarn,"=");
        ptr=ptr->next;
    }
}

void DNA_setbone(struct DNA *DNA,list *list){
    node *ptr=list->head;
    while(ptr!=NULL){
        struct protein* prot = new protein;
        protein_init(prot);
        node *_ptr=((struct protein*)(ptr->data))->compound->head;
        while(_ptr!=NULL){
            struct compound* comp = new compound;
            compound_init(comp);
            for(int i=0;i<((struct compound*)(_ptr->data))->len;i++){
                comp->elem.elements[i]=((struct compound*)(_ptr->data))->elem.elements[i];
            }
            comp->len=((struct compound*)(_ptr->data))->len;
            comp->size=((struct compound*)(_ptr->data))->size;
            list_insertion(prot->compound,comp);
            _ptr=_ptr->next;
        }
        list_insertion(DNA->protein,prot);
        ptr=ptr->next;
    }
}
void DNA_setprot(struct DNA *DNA,list *proteinlist){
    node *ptr=DNA->codn->head;
    struct protein* prot = new protein;
    protein_init(prot);
    while(ptr!=NULL){
        struct compound* comp = new compound;
        compound_init(comp);
        for(int i=0;i<((struct codn*)(ptr->data))->compound->len;i++){
            comp->elem.elements[i]=((struct codn*)(ptr->data))->compound->elem.elements[i];
        }
        comp->len=((struct codn*)(ptr->data))->compound->len;
        comp->size=((struct codn*)(ptr->data))->compound->size;
        list_insertion(prot->compound,comp);
        ptr=ptr->next;
    }
    list_insertion(proteinlist,prot);
}

void DNA_free(struct DNA *DNA){
    if (DNA == NULL)return;
    
    if(DNA->protein!=NULL){
        node *ptr=DNA->protein->head;
        while(ptr!=NULL){
            protein_free((struct protein*)(ptr->data));
            ptr=ptr->next;
        }
    }
    node *ptr=DNA->codn->head;
    while(ptr!=NULL){
        codn_free((struct codn*)(ptr->data));
        ptr=ptr->next;
    }
    delete DNA->codn;
    delete DNA->protein;
    if (DNA != NULL)delete DNA;
}