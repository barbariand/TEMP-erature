
#pragma once
#include <Arduino.h>
#include <iostream>
#include "Display.hpp"
namespace hal {
inline void init(Display* amoled) {
  Serial.begin(115200);
  // delay(200);

  if (!amoled->init()) {
    std::cout<<"Failed to init LilyGO AMOLED."<<std::endl;
    while (true)
      delay(1000);
  }
}
inline void sleep(int sleep_delay) {
  delay(sleep_delay);
}

}  // namespace hal
