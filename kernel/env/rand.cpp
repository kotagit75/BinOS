#include "rand.hpp"

int seed=8;

#define A 1103515245
#define B 12345
#define M (long)4294967296
#define abs(a) a<0?(-a):(a)

int next_ran=0;

void next_rand(){
    int x=(A*next_ran+B)%M;
    next_ran=abs(x);
}

int Krand(){
    next_rand();
    return next_ran;
}
void Ksrand(int seed_){
    seed=seed_;
    next_ran=seed;
}