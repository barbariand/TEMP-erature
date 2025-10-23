
#pragma once
#include <Arduino.h>
#include <cstdio>
#include "Display.hpp"
namespace hal {
inline void init(Display* amoled) {
  Serial.begin(115200);
  // delay(200);

  if (!amoled->init()) {
    Serial.println("Failed to init LilyGO AMOLED.");
    while (true)
      delay(1000);
  }
}
inline void sleep(int sleep_delay) {
  delay(sleep_delay);
}

int printf(const char* format, ...) {
  return Serial.printf(format, __builtin_va_list());
}
}  // namespace hal
