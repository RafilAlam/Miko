#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#define PRICONTEXT "\x1b[?1049l"
#define SECCONTEXT "\x1b[?1049h"

class Screen {
  public:
    std::string drawbuffer;
    Screen();
    inline std::string mvcursor(int x, int y);
    void render();

  private:
    void drawbox(int x, int y, int w, int h);
};
