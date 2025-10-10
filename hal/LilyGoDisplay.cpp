#include <Arduino.h>

#include "LilyGo/LV_Helper.h"
#include "LilyGo/LilyGo_AMOLED.h"
#include "LilyGoDisplay.h"
namespace hal {
using ::beginLvglHelperDMA;
LilyGoDisplay::LilyGoDisplay() {
  amoled = new LilyGo_AMOLED();
}
LilyGoDisplay::~LilyGoDisplay() {
  delete amoled;
}
bool LilyGoDisplay::init() {
  bool init = amoled->begin();
  if (init) {
    beginLvglHelperDMA(*amoled);
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
}  // namespace hal
