#pragma once

#ifdef WASM_BUILD

#include "IDisplay.hpp"
#include "lvgl.h"
extern "C" {

void wasm_bridge_init();

void wasm_bridge_flush(lv_display_t* disp, const lv_area_t* area,
                       uint8_t* color_p);

void wasm_bridge_get_input_data(uint32_t* out_state, int32_t* out_x,
                                int32_t* out_y);
}

namespace hal {

class WASMDisplay : public IDisplay {
 public:
  WASMDisplay();
  virtual ~WASMDisplay() = default;

  virtual bool init() override;
  virtual void setBrightness(uint8_t brightness) override;
  virtual void sleep() override;
  virtual void wakeup() override;
  virtual int handle_events() override;

 private:
  static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                       uint8_t* color_p);

  static void read_cb(lv_indev_t* indev, lv_indev_data_t* data);

  static lv_color_t buf1[800 * 600];
  static lv_display_t* disp;
  static lv_indev_t* indev;
};

}  // namespace hal

#endif
