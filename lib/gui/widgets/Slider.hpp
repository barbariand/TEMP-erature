#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Slider : public Widget, public PartIndicatorProperties<Slider> {
 private:
  explicit Slider(BaseWidget* parent) : Widget(lv_slider_create, parent) {}

 public:
  static std::shared_ptr<Slider> create(BaseWidget& parent) {
    auto new_slider = std::shared_ptr<Slider>(new Slider(&parent));
    parent.register_child(new_slider);
    return new_slider;
  }

  Slider& set_range(int32_t min, int32_t max) {
    if (m_obj)
      lv_slider_set_range(m_obj, min, max);
    return *this;
  }

  Slider& set_value(int32_t value, lv_anim_enable_t anim) {
    if (m_obj)
      lv_slider_set_value(m_obj, value, anim);
    return *this;
  }

  int32_t get_value() const { return m_obj ? lv_slider_get_value(m_obj) : 0; }
};
}  // namespace LVGL_Wrapper
