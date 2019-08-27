#ifndef _REQUIREMENTS_
#define _REQUIREMENTS_

#include <iostream>
#include "Vector.h"

#endif

namespace basic_vector {
  // default constructor, 0-vector
  Vector::Vector(): Vector(0, 0, 0) {}

  /*Vector::Vector(const Vector &v){
    std::cout << "calling cctor" << std::endl;
    Vector(v.vec[0], v.vec[1], v.vec[2]);
  }*/

  // initialize vector fields to provided parameters
  Vector::Vector(float x, float y, float z) {
    this->vec = new float[3];
    if(this->vec == nullptr){
      std::cout << "mem allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    }
    this->vec[0] = x;
    this->vec[1] = y;
    this->vec[2] = z;
  }

  Vector::~Vector(){
    delete[] vec;
  }

  // member function += operator override
  Vector &Vector::operator+=(const Vector &v2){
    this->vec[0] += v2.vec[0];
    this->vec[1] += v2.vec[1];
    this->vec[2] += v2.vec[2];
    return *this;
  }

  // member function += operator override
  Vector &Vector::operator-=(const Vector &v2){
    this->vec[0] -= v2.vec[0];
    this->vec[1] -= v2.vec[1];
    this->vec[2] -= v2.vec[2];
    return *this;
  }

  Vector &Vector::operator=(const Vector &v){
    this->vec[0] = v.vec[0];
    this->vec[1] = v.vec[1];
    this->vec[2] = v.vec[2];
    return *this;
  }

  Vector operator+(const Vector &v1, const Vector &v2){
    // assign new result vector to one of the
    // two to be added
    Vector res;

    res = v1;
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

  Vector operator*(const Vector &v1, double scalar) {
    float * vec;
    float s_x, s_y, s_z;
    vec = v1.getVec();
    s_x = vec[0] * scalar;
    s_y = vec[1] * scalar;
    s_z = vec[2] * scalar;
    Vector res(s_x, s_y, s_z);
    return res;
  }

  Vector operator*(double scalar, const Vector &v1) {
    return v1 * scalar;
  }

  float operator*(const Vector &v1, const Vector &v2) {
    float *vec1, *vec2;
    vec1 = v1.getVec();
    vec2 = v2.getVec();
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
  }

  float * Vector::getVec() const {
    return this->vec;
  }


  std::ostream &operator<<(std::ostream &out, const Vector &v){
    out << "(" << v.vec[0] << "," << v.vec[1] << "," << v.vec[2] << ")";
    return out;
  }
}
