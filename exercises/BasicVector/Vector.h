namespace basic_vector {
  class Vector {
  public:
    // default constructor, initialize zero-vector
    Vector();
    // constructor with provided values for vector
    Vector(float x, float y, float z);

    Vector &operator+=(const Vector &);
    Vector &operator-=(const Vector &);

    // friend keyword indicates this non-member function can access
    // private class data from the provided class
    friend std::ostream &operator<<(std::ostream &out, const Vector &v);

  private:
    float x, y, z;
  };

  // non-member operator overrides, overwrite the + and - operators
  // for vectors, while returning a new vector and not manipulating
  // the state of the original two vectors
  Vector operator+(const Vector &, const Vector &);
  Vector operator-(const Vector &, const Vector &);
}
