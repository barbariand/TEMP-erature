#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Bar : public Widget, public PartIndicatorProperties<Bar> {
 private:
  explicit Bar(BaseWidget* parent) : Widget(lv_bar_create, parent) {}

 public:
  static std::shared_ptr<Bar> create(BaseWidget& parent) {
    auto new_bar = std::shared_ptr<Bar>(new Bar(&parent));
    parent.register_child(new_bar);
    return new_bar;
  }

  Bar& set_range(int32_t min, int32_t max) {
    if (m_obj)
      lv_bar_set_range(m_obj, min, max);
    return *this;
  }

  Bar& set_value(int32_t value, lv_anim_enable_t anim) {
    if (m_obj)
      lv_bar_set_value(m_obj, value, anim);
    return *this;
  }

  int32_t get_value() const { return m_obj ? lv_bar_get_value(m_obj) : 0; }
};

}  // namespace LVGL_Wrapper
