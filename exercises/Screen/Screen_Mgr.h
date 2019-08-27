#ifndef SCREEN_DEF
#define SCREEN_DEF

#include "Screen.h"

#endif

namespace screen {
  class Screen_Mgr {
  friend std::ostream &operator<<(std::ostream &, const Screen_Mgr &);
  public:
    Screen_Mgr();
    Screen_Mgr(std::vector<Screen>);

    int get_num_screens() const;
    std::vector<Screen> get_screens() const;
    void add_screen(Screen);
  private:
    std::vector<Screen> screens;
  };
}
