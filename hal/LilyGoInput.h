#pragma once
#include "IInput.h"
#include "LilyGo/LilyGo_AMOLED.h"
#include <TFT_eSPI.h>
class LilyGoInput : public IInput {
 public:
  LilyGoInput(LilyGo_AMOLED* display);
  void init() override;
  void read(lv_indev_drv_t* drv, lv_indev_data_t* data) override;

 private:
  LilyGo_AMOLED* amoled;
  TFT_eSPI tft;
  lv_coord_t last_x = 0;
  lv_coord_t last_y = 0;
};
