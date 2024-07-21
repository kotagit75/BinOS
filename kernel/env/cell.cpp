
#include "env/cell.hpp"

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <stdlib.h>

#include "env/cushionmodel.hpp"
#include "env/list.hpp"
#include "env/DNA.hpp"
#include "env/protein.hpp"
#include "env/codn.hpp"
#include "env/rand.hpp"
#include "env/eval.hpp"

cell::cell(int lifespan){
    this->remaining_life=this->lifespan=lifespan;
    fitness=50;
    this->DNA = new struct DNA;
    DNA_init(DNA);
    this->protein = new list;
    list_init(protein);
    disable();
}

void cell::arrangement(){
    if(0){
        int binary1_1[]={0x48,0x89,0xf8};
        int binary1_2[]={0xbf,0x4b,0x00,0x00,0x00};
        int binary1_3[]={0xbb,0x00,0x00,0x00,0x00};
        int binary2[]={0xff,0xd0};
        int binary3[]={0xc3};
        {
            struct protein *protein1=proteinINIT;
            struct compound *comp1=compoundINIT;
            for (int comp_c = 0; comp_c < 3; comp_c++){
                compound_code_push(comp1,binary1_1[comp_c]);
            }
            protein_compound_push(protein1,comp1);
            cell_protein_push(protein1);
        }
        {
            struct protein *protein1=proteinINIT;
            struct compound *comp1=compoundINIT;
            for (int comp_c = 0; comp_c < 5; comp_c++){
                compound_code_push(comp1,binary1_2[comp_c]);
            }
            protein_compound_push(protein1,comp1);
            cell_protein_push(protein1);
        }
        {
            struct protein *protein1=proteinINIT;
            struct compound *comp1=compoundINIT;
            for (int comp_c = 0; comp_c < 5; comp_c++){
                compound_code_push(comp1,binary1_3[comp_c]);
            }
            protein_compound_push(protein1,comp1);
            cell_protein_push(protein1);
        }
        {
            struct protein *protein1=proteinINIT;
            struct compound *comp1=compoundINIT;
            for (int comp_c = 0; comp_c < 2; comp_c++){
                compound_code_push(comp1,binary2[comp_c]);
            }
            protein_compound_push(protein1,comp1);
            cell_protein_push(protein1);
        }
        {
            struct protein *protein1=proteinINIT;
            struct compound *comp1=compoundINIT;
            for (int comp_c = 0; comp_c < 1; comp_c++){
                compound_code_push(comp1,binary3[comp_c]);
            }
            protein_compound_push(protein1,comp1);
            cell_protein_push(protein1);
        }
    }else{
        int binarycINT[]={0xc3};
        struct protein *protein1=proteinINIT;
        struct compound *comp1=compoundINIT;
        for (int comp_c = 0; comp_c < 1; comp_c++){
            compound_code_push(comp1,binarycINT[comp_c]);
        }
        protein_compound_push(protein1,comp1);
        cell_protein_push(protein1);
    }
}

cell::~cell(){

}

char bincary_func[100];

int cell::run(){
    if(!is_use)return 0;
    EC.log(LInfo,"cell::run\n");
    metabolism_and_motion();
    if(fitness == 200){
        death();
        return 0;
    }
    if(fitness==100){
        bio_env_success(this);
    }
    if(grow()==1)proliferation();
    remaining_life--;
    if(remaining_life==0){
        death();
    }
    return 1;
}

void cell::metabolism_and_motion(){
    EC.log(LInfo,"metabolism_and_motion\n");
    bio_env *be=GetBE();
    for(int i=0;i<100;i++){
        be->codedata_bond[i]=-1;
    }
    node *ptr=protein->head;
    be->codedata_bond_len=0;
    while(ptr!=NULL){
        protein_bond(((struct protein*)(ptr->data)),be);
        ptr=ptr->next;
    }
    for(int i=0;!(be->codedata_bond[i]==-1 || i>=100);i++){
        bincary_func[i]=(char)be->codedata_bond[i];
        EC.log(LInfo,"0x%x ",be->codedata_bond[i]);
    }
    EC.log(LInfo,"\n");
    EC.log(LInfo,"RUN\n");
    int cellbin_Eflag=EC.eval->run(bincary_func);
    fitness=EC.eval->Eval(bincary_func,cell_result{cellbin_Eflag});
    EC.log(LInfo,"fitness:%d\n",fitness);
}

int cell::grow(){
    EC.log(LInfo,"cell::grow\n");
    struct compound *comp = bio_env_compound_pop_FILTER_random(compound_TYPE_CODE);
    if(comp==(struct compound *)-1)goto proliferationisOK;
    else if(!(Krand() / (RAND_MAX / 101 + 1) <= fitness)){
        bio_env_compound_push(comp);
        goto proliferationisOK;
    }else{
        node *ptr=DNA->codn->head;
        int isgrow=0;
        while(ptr!=NULL){
            if(codn_setcomp(((struct codn*)(ptr->data)),comp)==1){isgrow=1;break;}
            ptr=ptr->next;
        }
        if(isgrow==0){
            bio_env_compound_push(comp);
        }
    }
    proliferationisOK:
    node *ptr=DNA->codn->head;
    while(ptr!=NULL){
        if(((struct codn*)(ptr->data))->compound==NULL)goto no;
        ptr=ptr->next;
    }
    return 1;
    no:
    return 0;
}
void cell::proliferation(){
    EC.log(LInfo,"cell::proliferation\n");
    struct cell *newcell=new cell(lifespan);
    node *ptr=DNA->codn->head;
    int count =0;
    while(ptr!=NULL){
        struct codn *cod=new codn;
        codn_init(cod);
        struct compound *comp;
        if(count < 10 && (Krand()%100)>=fitness && (comp = bio_env_compound_pop_FILTER_random(compound_TYPE_CODE))!=(struct compound*)-1){
            for (int i = 0; i < comp->len; i++) {
                cod->data[i] = comp->elem.elements[i];
            }
            cod->compound=comp;
            list_insertion(newcell->DNA->codn,cod);
            count++;
        }else{
            for (int i = 0; i < ((struct codn*)(ptr->data))->compound->len; i++) {
                cod->data[i] = ((struct codn*)(ptr->data))->data[i];
            }
            cod->compound=((struct codn*)(ptr->data))->compound;
            ((struct codn*)(ptr->data))->compound=NULL;
            list_insertion(newcell->DNA->codn,cod);
            ptr=ptr->next;
        }
    }
    ptr=newcell->DNA->codn->head;
    struct protein* prot = proteinINIT;
    while(ptr!=NULL){
        list_insertion(prot->compound,((struct codn*)(ptr->data))->compound);
        ((struct codn*)(ptr->data))->compound=NULL;
        ptr=ptr->next;
    }
    list_insertion(newcell->protein,prot);
    int codn_n=0;
    ptr=newcell->DNA->codn->head;
    while(ptr!=NULL){
        codn_n++;
        ptr=ptr->next;
    }
    newcell->lifespan=codn_n*40;
    newcell->remaining_life=newcell->lifespan;
    newcell->enable();
    bio_env_cell_push(newcell);
}

/*void cell::proliferation(){
    EC.log(LInfo,"cell::proliferation\n");
    struct cell *newcell=new cell(lifespan,main_task,success);
    node *ptr=protein->head;
    node *codn_ptr=DNA->codn->head;
    node *_ptr;
    while(ptr!=NULL){
        struct protein *protein=new struct protein;
        protein_init(protein);
        if((EC.rand()%100)>=fitness){
            struct compound *comp = bio_env_compound_pop_FILTER_random(compound_TYPE_CODE);
            if(comp!=NULL){
                protein_compound_push(protein,comp);
                newcell->cell_protein_push(protein);
                continue;
            }else{
                _ptr=((struct protein*)(ptr->data))->compound->head;
                while(_ptr!=NULL){
                    protein_compound_push(protein,((struct codn *)(codn_ptr->data))->compound);
                    codn_ptr=codn_ptr->next;
                    _ptr=_ptr->next;
                }
            }
        }else{
            _ptr=((struct protein*)(ptr->data))->compound->head;
            while(_ptr!=NULL){
                protein_compound_push(protein,((struct codn *)(codn_ptr->data))->compound);
                codn_ptr=codn_ptr->next;
                _ptr=_ptr->next;
            }
        }
        newcell->cell_protein_push(protein);
        ptr=ptr->next;
    }
    DNA_setbone(newcell->DNA,newcell->protein);
    int codn_n=0;
    ptr=DNA->codn->head;
    while(ptr!=NULL){
        codn_n++;
        ((struct codn *)(ptr->data))->compound=NULL;
        ptr=ptr->next;
    }
    newcell->lifespan=codn_n*40;
    newcell->remaining_life=newcell->lifespan;
    newcell->enable();
    bio_env_cell_push(newcell);
}*/
void cell::death(){
    EC.log(LInfo,"cell::death\n");
    node *ptr=protein->head;
    node *_ptr;
    while(ptr!=NULL){
        _ptr=((struct protein *)(ptr->data))->compound->head;
        while(_ptr!=NULL){
            bio_env_compound_push(((struct compound *)(_ptr->data)));
            _ptr=_ptr->next;
        }
        ptr=ptr->next;
    }
    ptr=DNA->codn->head;
    while(ptr!=NULL){
        if(((struct codn *)(ptr->data))->compound!=NULL)bio_env_compound_push(((struct codn *)(ptr->data))->compound);
        ptr=ptr->next;
    }
    disable();
}
void cell::print(){
    node *ptr=protein->head;
    EC.log(LWarn,"{");
    while(ptr!=NULL){
        protein_print(((struct protein*)(ptr->data)));
        if(ptr->next!=NULL)EC.log(LWarn,",");
        ptr=ptr->next;
    }
    EC.log(LWarn,"}");
    DNA_print(DNA);
    EC.log(LWarn,"(life:%d/%d,fitness:%d)",remaining_life,lifespan,fitness);
    EC.log(LWarn,"\n");
}

void cell::enable(){
    is_use=1;
}
void cell::disable(){
    is_use=0;
}
void cell::cell_protein_push(struct protein *protein){
    node *ptr=protein->compound->head;
    int i;
    while(ptr!=NULL){
        struct codn *codn=new struct codn;
        codn_init(codn);
        struct compound *comp=((struct compound*)(ptr->data));
        for(i=0;i<comp->len;i++){
            codn->data[i]=comp->elem.elements[i];
        }
        DNA_codn_push(DNA,codn);
        ptr=ptr->next;
    }
    list_insertion(this->protein,protein);
}