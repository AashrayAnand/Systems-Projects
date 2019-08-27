#include <vector>
#include <iostream>

#include "Screen_Mgr.h"

namespace screen {
  Screen_Mgr::Screen_Mgr() {
    std::vector<Screen> emtpy;
    Screen_Mgr(empty);
  }

  Screen_Mgr::Screen_Mgr(std::vector<Screen> screens){
    this->screens = screens;
  }

  void Screen_Mgr::add_screen(Screen screen){
    this->screens.push_back(screen);
  }

  int Screen_Mgr::get_num_screens() const {
    return this->screens.size();
  }

  std::vector<Screen> Screen_Mgr::get_screens() const {
    return this->screens;
  }

  std::ostream &operator<<(std::ostream & os, const Screen_Mgr & manager){
    int i;
    std::vector<Screen> screens;
    screens = manager.get_screens();
    for(i = 0; i < manager.get_num_screens(); i++){
      std::cout << screens[i];
    }

    return os;
  }
}
