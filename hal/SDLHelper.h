
#pragma once
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "Display.h"
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
int printf(const char* format, ...) {
  // The compiler directly forwards the variable arguments to the
  // standard C library's printf function (from <cstdio>).
  return ::printf(format, __builtin_va_list());
}
}  // namespace hal
