
#pragma once
#include <Arduino.h>
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
  va_list args;
  va_start(args, format);
  int result = Serial.vprintf(format, args);
  va_end(args);
  return result;
}
}  // namespace hal
