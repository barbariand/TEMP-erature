#ifdef WASM_BUILD

#include "WASMDisplay.hpp"
#include <stdio.h>
lv_color_t hal::WASMDisplay::buf1[800 * 600];
lv_display_t* hal::WASMDisplay::disp = nullptr;
lv_indev_t* hal::WASMDisplay::indev = nullptr;

namespace hal {

WASMDisplay::WASMDisplay() {}

bool WASMDisplay::init() {
  lv_init();
  wasm_bridge_init();

  disp = lv_display_create(800, 600);
  if (!disp) {
    return false;
  }

  lv_display_set_default(disp);

  uint32_t buf_size_bytes = 800 * 600 * sizeof(lv_color_t);
  lv_display_set_buffers(disp, buf1, NULL, buf_size_bytes,
                         LV_DISPLAY_RENDER_MODE_FULL);

  lv_display_set_flush_cb(disp, WASMDisplay::flush_cb);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB888);

  indev = lv_indev_create();
  if (!indev) {
    return false;
  }
  lv_indev_enable(indev, true);
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, WASMDisplay::read_cb);
  lv_indev_set_display(indev, disp);

  lv_group_t* group = lv_group_create();
  lv_group_set_default(group);

  return true;
}

void WASMDisplay::flush_cb(lv_display_t* disp, const lv_area_t* area,
                           uint8_t* color_p) {
  wasm_bridge_flush(disp, area, color_p);
}

void WASMDisplay::read_cb(lv_indev_t* indev, lv_indev_data_t* data) {
  uint32_t rust_state = 0;
  int32_t rust_x = 0;
  int32_t rust_y = 0;

  wasm_bridge_get_input_data(&rust_state, &rust_x, &rust_y);

  if (rust_state == (uint32_t)LV_INDEV_STATE_PRESSED) {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = (lv_coord_t)rust_x;
    data->point.y = (lv_coord_t)rust_y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void WASMDisplay::setBrightness(uint8_t brightness) {
  (void)brightness;
}

void WASMDisplay::sleep() {}

void WASMDisplay::wakeup() {}

int WASMDisplay::handle_events() {
  return 0;
}

}  // namespace hal

#endif
