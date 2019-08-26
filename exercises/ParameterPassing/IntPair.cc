#include "IntPair.h"

IntPair::IntPair(int x, int y): i(x), j(y){}

void IntPair::Get(int *x, int *y){
  *x = i;
  *y = j;
}

void IntPair::Set(int x, int y){
  i = x;
  j = y;
}
