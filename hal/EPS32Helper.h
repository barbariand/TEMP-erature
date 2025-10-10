#include <Arduino.h>
#include "HAL.h"
namespace hal {
inline void init(Display *amoled) {
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
}  // namespace hal
