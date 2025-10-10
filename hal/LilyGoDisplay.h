#pragma once
#include "IDisplay.h"
#include "LilyGo/LilyGo_AMOLED.h"

class LilyGoDisplay : public IDisplay {
 public:
  LilyGoDisplay();
  ~LilyGoDisplay();
  void init() override;
  void flush(lv_disp_drv_t* drv, const lv_area_t* area,
             lv_color_t* color_p) override;
  void setBrightness(uint8_t brightness) override;
  void sleep() override;
  void wakeup() override;

 private:
  LilyGo_AMOLED* amoled;
};
