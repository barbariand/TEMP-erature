#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class LED : public Widget {
 private:
  explicit LED(BaseWidget* parent) : Widget(lv_led_create, parent) {}

 public:
  static std::shared_ptr<LED> create(BaseWidget& parent) {
    auto new_led = std::shared_ptr<LED>(new LED(&parent));
    parent.register_child(new_led);
    return new_led;
  }

  LED& set_color(lv_color_t color) {
    if (m_obj)
      lv_led_set_color(m_obj, color);
    return *this;
  }

  LED& set_brightness(uint8_t bright) {
    if (m_obj)
      lv_led_set_brightness(m_obj, bright);
    return *this;
  }

  LED& on() {
    if (m_obj)
      lv_led_on(m_obj);
    return *this;
  }

  LED& off() {
    if (m_obj)
      lv_led_off(m_obj);
    return *this;
  }

  LED& toggle() {
    if (m_obj)
      lv_led_toggle(m_obj);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
