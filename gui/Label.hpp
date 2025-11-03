#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Label : public Widget {
 public:
  Label() : Widget() {}
  Label& create(BaseWidget& parent) {
    wrap(lv_label_create(parent.raw()));
    return *this;
  }


  Label& set_text(const char* text) {
    if (m_obj)
      lv_label_set_text(m_obj, text);
    return *this;
  }

  Label& set_long_mode(lv_label_long_mode_t mode) {
    if (m_obj)
      lv_label_set_long_mode(m_obj, mode);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
