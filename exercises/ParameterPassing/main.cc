// Copyright 2019 Aashray Anand
#include <iostream>
#include "IntPair.h"

using std::cin;
using std::cout;
using std::endl;

void increment(IntPair);

int main(int argc, char **argv){
  int x, y;
  IntPair pair(1,2);
  increment(pair);
  pair.Get(&x, &y);

  if(x == 1 && y == 2){
    cout << "pass by value" << endl;
  } else {
    cout << "pass by reference" << endl;
  }
}

void increment(IntPair pair){
  int x, y, x_1, y_1;
  pair.Get(&x, &y);
  cout << x << " " << y << endl;
  pair.Set(x + 1, y + 1);
  pair.Get(&x, &y);
  cout << x << " " << y << endl;
}
