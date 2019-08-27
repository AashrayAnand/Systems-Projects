#include <iostream>
#include "Vector.h"

int main(){
  basic_vector::Vector v1(1,2,3);
  basic_vector::Vector v2;
  v2 = v1;
  std::cout << v1 << std::endl;
  std::cout << v2 << std::endl;
  std::cout << "dot product of v1 and v2 is " << v1 * v2 << std::endl;
  basic_vector::Vector v3 = v1 + v2;
  basic_vector::Vector scaled_v3_vector_first = v3 * 4.0;
  basic_vector::Vector scaled_v3_vector_second = 4.0 * v3;
  std::cout << "we can multiply a scalar by a vector " << scaled_v3_vector_first << std::endl;
  std::cout << "with the scalar as arg 1 or 2 " << scaled_v3_vector_second << std::endl;
  std::cout << v3 << std::endl;
  return 0;
}
