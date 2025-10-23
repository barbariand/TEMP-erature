
#pragma once
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "Display.hpp"
namespace hal {
inline void init(Display* amoled) {

  if (!amoled->init()) {
    std::cout << "failed to init, exiting";
    std::exit(0);
  }
}
inline void sleep(int sleep_delay) {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_delay));
}
inline int printf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int result = ::vprintf(format, args);
  va_end(args);
  return result;
}
}  // namespace hal
