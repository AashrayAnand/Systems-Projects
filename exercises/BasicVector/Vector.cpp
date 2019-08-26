#include <iostream>
#include "Vector.h"

namespace basic_vector {
  // default constructor, 0-vector
  Vector::Vector(): x(0), y(0), z(0) {}

  // initialize vector fields to provided parameters
  Vector::Vector(float x, float y, float z): x(x), y(y), z(z) {}

  // member function += operator override
  Vector &Vector::operator+=(const Vector &v2){
    this->x += v2.x;
    this->y += v2.y;
    this->z += v2.z;
    return *this;
  }

  // member function += operator override
  Vector &Vector::operator-=(const Vector &v2){
    this->x -= v2.x;
    this->y -= v2.y;
    this->z -= v2.z;
    return *this;
  }

  Vector operator+(const Vector &v1, const Vector &v2){
    // assign new result vector to one of the
    // two to be added
    Vector res = v1;

    // calling +=  on res will invoke underlying
    // member function implementation of +=
    res += v2;
    return res;
  }

  Vector operator-(const Vector &v1, const Vector &v2){
    // assign new result vector to one of the
    // two to be added
    Vector res = v1;

    // calling +=  on res will invoke underlying
    // member function implementation of +=
    res -= v2;
    return res;
  }

  std::ostream &operator<<(std::ostream &out, const Vector &v){
    out << "(" << v.x << "," << v.y << "," << v.z << ")";
    return out;
  }
}
