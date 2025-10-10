#include "LilyGoDisplay.h"

LilyGoDisplay::LilyGoDisplay() {
  amoled = new LilyGo_AMOLED();
}
LilyGoDisplay::~LilyGoDisplay() {
    delete amoled;
}
void LilyGoDisplay::init() {
  amoled->begin();
}

void LilyGoDisplay::flush(lv_disp_drv_t* drv, const lv_area_t* area,
                          lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  amoled->pushColors(area->x1, area->y1, w, h, (uint16_t*)color_p);
  lv_disp_flush_ready(drv);
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
