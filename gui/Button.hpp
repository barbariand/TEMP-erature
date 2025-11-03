#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Button : public Widget {
 public:
  Button() : Widget() {}
  Button& create(BaseWidget& parent) {
    wrap(lv_button_create(parent.raw()));
    return *this;
  }
};

}  // namespace LVGL_Wrapper
