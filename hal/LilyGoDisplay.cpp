#if defined(ARDUINO_ARCH_ESP32)
#include <Arduino.h>

#include "LilyGo/LV_Helper.h"
#include "LilyGo/LilyGo_AMOLED.h"
#include "LilyGoDisplay.hpp"
namespace hal {
using ::beginLvglHelper;
LilyGoDisplay::LilyGoDisplay() {
  amoled = new LilyGo_AMOLED();
}
LilyGoDisplay::~LilyGoDisplay() {
  delete amoled;
}
bool LilyGoDisplay::init() {
  bool init = amoled->begin();
  if (init) {
    beginLvglHelper(*amoled);
  }
  return init;
}

void LilyGoDisplay::setBrightness(uint8_t brightness) {
  amoled->setBrightness(brightness);
}

void LilyGoDisplay::sleep() {
  amoled->sleep(false);
}

void LilyGoDisplay::wakeup() {
  amoled->disp_wakeup();
}
int LilyGoDisplay::handle_events() {
  return 0;
}
}  // namespace hal
#endif
