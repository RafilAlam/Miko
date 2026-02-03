#include <ui.hpp>
#include <cstdlib>
#include <csignal>

std::string Screen::mvcursor(int x, int y) {
  std::string xs = std::to_string(x);
  std::string ys = std::to_string(y);
  return "\033[" + ys + ";" + xs + "H";
}

void exitHandler() {
  std::cout << PRICONTEXT << std::flush;
}

void signalHandler(int signum) {
  std::exit(signum);
}

void Screen::drawbox(int x, int y, int w, int h) {
  this->drawbuffer += this->mvcursor(x, y) + "╭";
  this->drawbuffer += this->mvcursor(x + w - 1, y) + "╮";
  this->drawbuffer += this->mvcursor(x, y + h - 1) + "╰";
  this->drawbuffer += this->mvcursor(x + w - 1, y + h - 1) + "╯";

  for (int i=1; i<w-1; ++i) {
    this->drawbuffer += this->mvcursor(x + i, y) + "─";
    this->drawbuffer += this->mvcursor(x + i, y + h - 1) + "─";
  }

  for (int i=1; i<h-1; ++i) {
    this->drawbuffer += this->mvcursor(x, y + i) + "│";
    this->drawbuffer += this->mvcursor(x + w - 1, y + i) + "│";
  }
}

Screen::Screen(): drawbuffer(SECCONTEXT) {
  std::atexit(exitHandler);
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
}

void Screen::render() {
  std::cout << drawbuffer << std::flush;
}
