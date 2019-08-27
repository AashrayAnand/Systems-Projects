#include <iostream>

#ifndef SCREEN_DEF
#define SCREEN_DEF

#include "Screen.h"

#endif
namespace screen {
  Screen::Screen(int width, int height){
    this->width = width;
    this->height = height;
    this->pixels = width * height;
  }

  int Screen::get_pixels() const {
    access_ctr++;
    return this->pixels;
  }

  int Screen::get_width() const {
    access_ctr++;
    return this->width;
  }

  int Screen::get_height() const {
    access_ctr++;
    return this->height;
  }

  std::ostream &operator<<(std::ostream &os, const Screen &screen) {
    int i, j;
    int width, height;
    width = screen.get_width();
    height = screen.get_height();
    for(i = 0; i < width; i++){
      os << "=";
    }
    os << "\n";
    for(j = 0; j < height - 2; j++){
      os << "|";
      for(i = 0; i < width - 2; i++){
        os << " ";
      }
      os << "|\n";
    }
    for(i = 0; i < width; i++){
      os << "=";
    }
    os << "\n";
    os << "width is: " << screen.get_width() << "\n";
    os << "height is: " << screen.get_height() << "\n";
    os << "pixels is: " << screen.get_pixels() << "\n";
    return os;
  }
}
