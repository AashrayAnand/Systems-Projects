#include <iostream>
#include <vector>

#ifndef SCREEN_DEF
#define SCREEN_DEF

#include "Screen.h"

#endif
#include "Screen_Mgr.h"

using namespace screen;

int main(){
  int i;
  std::vector<Screen> v;
  Screen screen(10, 20);
  Screen screen2(15, 15);
  Screen screen3(9, 3);
  Screen screen4(4, 22);
  v.push_back(screen);
  v.push_back(screen2);
  v.push_back(screen3);
  v.push_back(screen4);
  Screen_Mgr manager(v);
  std::cout << manager << std::endl;
  return 0;
}
