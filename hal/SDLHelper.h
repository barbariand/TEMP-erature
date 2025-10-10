#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "HAL.h"
namespace hal {
inline void init(Display* amoled) {

  if (!amoled->init()) {
    std::cout << "Failed to init, exiting";
    std::exit(0);
  }
}
inline void sleep(int sleep_delay) {
  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_delay));
}
}  // namespace hal
