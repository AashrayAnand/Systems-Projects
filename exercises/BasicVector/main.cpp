#include <iostream>
#include "Vector.h"


int main(){
  basic_vector::Vector v1(1,2,3);
  basic_vector::Vector v2(1,2,3);
  std::cout << v1 << std::endl;
  std::cout << v2 << std::endl;
  basic_vector::Vector v3 = v1 + v2;
  std::cout << v3 << std::endl;
  return 0;
}
