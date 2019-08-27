namespace screen {
  class Screen {
  friend std::ostream &operator<<(std::ostream &, const Screen &);
  public:
    Screen() = default;
    Screen(int, int);

    int get_pixels() const;
    int get_width() const;
    int get_height() const;
  private:
    mutable size_t access_ctr; // # of times class has been accessed
                               // by any member function
    int width = 0;
    int height = 0;
    int pixels = 0;
  };
}
