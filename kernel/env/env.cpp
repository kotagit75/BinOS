#include "env/env.hpp"

#include <vector>
#include <stdlib.h>
#include <cstring>

#include "env/compound.hpp"
#include "env/cushionmodel.hpp"
#include "env/rand.hpp"

namespace{
    bio_env *be;
}

void InitializeBio_Env(int cycleMAX){
  Ksrand(201);
  be = new struct bio_env;
  be->cycle=0;
  be->cycleMAX=cycleMAX;
  for (int i=0;i<CELL_MAX;i++){
    cell *cell_=new cell(DEFAULT_lifespan);
    cell_->arrangement();
    be->cell.push_back(cell_);
  }
  be->cell[1]->enable();
  be->compound=new list;
  list_init(be->compound);
  int binary1_1[]={0x48,0x89,0xf8};
  int binary1_2[]={0xbf,0x4b,0x00,0x00,0x00};
  int binary1_3[]={0xBB,0x00,0x00,0x00,0x00};
  int binary2[]={0xff,0xd0};
  int binary3[]={0xc3};
  int comp_NUM_MAX=20;
  for (int j = 0; j < comp_NUM_MAX; j++){
    struct compound *comp = compoundINIT;
    for (int i = 0; i < 3; i++){
      compound_code_push(comp,binary1_1[i]);
    }
    list_insertion(be->compound,comp);
  }
  for (int j = 0; j < comp_NUM_MAX; j++){
    struct compound *comp = compoundINIT;
    for (int i = 0; i < 5; i++){
      compound_code_push(comp,binary1_2[i]);
    }
    list_insertion(be->compound,comp);
  }
  for (int j = 0; j < comp_NUM_MAX; j++){
    struct compound *comp = compoundINIT;
    for (int i = 0; i < 5; i++){
      compound_code_push(comp,binary1_3[i]);
    }
    list_insertion(be->compound,comp);
  }
  for (int j = 0; j < comp_NUM_MAX; j++){
    struct compound *comp = compoundINIT;
    for (int i = 0; i < 2; i++){
      compound_code_push(comp,binary2[i]);
    }
    list_insertion(be->compound,comp);
  }
  for (int j = 0; j < comp_NUM_MAX; j++){
    struct compound *comp = compoundINIT;
    for (int i = 0; i < 1; i++){
      compound_code_push(comp,binary3[i]);
    }
    list_insertion(be->compound,comp);
  }
}

void bio_env_main(Ecushion ec,int cycleMAX){
  InitializeEC(ec);InitializeBio_Env(cycleMAX);
  bio_env_print();bio_env_runcycle();bio_env_print();
}

void bio_env_success(cell *cell){
  EC.log(LWarn,"success!\n");
  cell->print();
  EC.success(cell);
  bio_env_finish();
}

void bio_env_finish(){
  EC.finish();
  while (1);
}

void bio_env_run(){
  EC.log(LWarn,"====cycle:%d====\n",be->cycle);
  int count=0;
  for (cell *cell_elem : be->cell){
    count+=cell_elem->run();
  }
  if(count==0){
    bio_env_finish();
  }else{
    for(int i=0;i<count;i++)
      EC.log(LWarn,"*");
    EC.log(LWarn,"\n");
  }
  be->cycle++;
}

void bio_env_runcycle(){
  for (int i = 0; i < be->cycleMAX; i++){
    bio_env_run();
  }
}

void bio_env_print(){
  EC.log(LWarn,"===========cycle:%d===========\n",be->cycle);
  EC.log(LWarn,"cell:\n");
  int count=0;
  for (cell *cell_elem : be->cell){
    if(cell_elem->is_use){
      EC.log(LWarn,"%d:  ",count);
      cell_elem->print();
    }
    count++;
  }
  EC.log(LWarn,"compound:\n");
  node *ptr=be->compound->head;
  while(ptr!=NULL){
      compound_print(((struct compound*)(ptr->data)));
      EC.log(LWarn,",");
      ptr=ptr->next;
  }
  EC.log(LWarn,"\n");
}

void bio_env_cell_push(cell *_cell){
  for (cell *cell_elem : be->cell){
    if(!(cell_elem->is_use)){
      memcpy(cell_elem,_cell,sizeof(cell));
      break;
    }
  }
}

void bio_env_compound_push(struct compound *comp){
    list_insertion(be->compound,comp);
}
void bio_env_compound_pop(struct compound *comp){
    list_deletion(be->compound,comp);
}

struct compound *bio_env_compound_pop_FILTER(int filter){
    node *ptr=be->compound->head;
    int len=0;
    while(ptr!=NULL){
        if(filter==compound_type(((struct compound*)(ptr->data)))){
            bio_env_compound_pop((struct compound*)(ptr->data));
            return (struct compound*)(ptr->data);
        }
        ptr=ptr->next;
    }
    return (struct compound*)-1;
}

struct compound *bio_env_compound_pop_FILTER_random(int filter){
    node *ptr=be->compound->head;
    int len=0;
    while(ptr!=NULL){
        if(filter==compound_type(((struct compound*)(ptr->data)))){
            len++;
        }
        ptr=ptr->next;
    }
    if(len==0)return (struct compound *)-1;
    struct compound* comps[len];
    int num=0;
    ptr=be->compound->head;
    while(ptr!=NULL){
        if(filter==compound_type(((struct compound*)(ptr->data)))){
            comps[num]=((struct compound*)(ptr->data));
            num++;
        }
        ptr=ptr->next;
    }
    int index=Krand()%len;
    bio_env_compound_pop(comps[index]);
    return comps[index];
}

bio_env *GetBE(){
  return be;
}