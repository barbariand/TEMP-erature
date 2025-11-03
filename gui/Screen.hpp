#pragma once
#include "Widget.hpp"
#include "lvgl.h"

namespace LVGL_Wrapper {

class Screen final : public Widget {
 private:
  Screen() : Widget(nullptr) {}

  ~Screen() override { m_obj = nullptr; }

  static Screen s_instance;
  static bool s_is_initialized;

 public:
  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  static Screen& getInstance() {
    if (!s_is_initialized) {
      s_instance.wrap(lv_scr_act());
      s_is_initialized = true;
    }
    return s_instance;
  }
  lv_display_t* get_display() {
    if (!m_obj)
      return nullptr;

    return lv_obj_get_disp(m_obj);
  }
  int horizontal_res() {
    return lv_display_get_horizontal_resolution(this->get_display());
  }
  int vertical_res() {
    return lv_display_get_vertical_resolution(this->get_display());
  }
};
}  // namespace LVGL_Wrapper
