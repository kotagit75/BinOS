#include "env/cushion/cushion.hpp"
#include "env/cell.hpp"

void EC_success(cell *cell){

}
void EC_finish(){
  while(1)asm volatile ("hlt");
}
