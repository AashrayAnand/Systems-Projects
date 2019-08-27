namespace basic_vector {
  class Vector {

  // friend keyword indicates this non-member function can access
  // private class data from the provided class
  friend std::ostream &operator<<(std::ostream &out, const Vector &v);

  public:

    // default constructor, initialize zero-vector
    Vector();
    // constructor with provided values for vector
    Vector(const float x, const float y, const float z);


    // destructor, will be empty
    ~Vector();

    // operator overloads used for assigning values
    // to vectors
    Vector &operator+=(const Vector &);
    Vector &operator-=(const Vector &);
    Vector &operator=(const Vector &);

    float * getVec() const;

  private:
    float* vec;
  };

  // non-member operator overrides, overwrite the + and - operators
  // for vectors, while returning a new vector and not manipulating
  // the state of the original two vectors
  Vector operator+(const Vector &, const Vector &);
  Vector operator-(const Vector &, const Vector &);
  Vector operator*(double, const Vector &);
  Vector operator*(const Vector &, double);
  float operator*(const Vector &, const Vector &);
}
