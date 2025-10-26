#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Label : public Widget {
 public:
  /**
   * @brief Default constructor for two-stage init.
   */
  Label() : Widget(nullptr) {}

  /**
   * @brief Creates a new Label immediately.
   */
  explicit Label(BaseWidget& parent) : Widget(lv_label_create(parent.raw())) {}

  /**
   * @brief Creates the LVGL object. Used for two-stage init.
   */
  Label& create(BaseWidget& parent) {
    wrap(lv_label_create(parent.raw()));
    return *this;
  }

  // --- Label-specific builder functions ---

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
