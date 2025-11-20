#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Scale : public Widget, public PartIndicatorProperties<Scale> {
 private:
  explicit Scale(BaseWidget* parent) : Widget(lv_scale_create, parent) {}

 public:
  static std::shared_ptr<Scale> create(BaseWidget& parent) {
    auto new_scale = std::shared_ptr<Scale>(new Scale(&parent));
    parent.register_child(new_scale);
    return new_scale;
  }

  Scale& set_mode(lv_scale_mode_t mode) {
    if (m_obj)
      lv_scale_set_mode(m_obj, mode);
    return *this;
  }

  Scale& set_range(int32_t min, int32_t max) {
    if (m_obj)
      lv_scale_set_range(m_obj, min, max);
    return *this;
  }

  Scale& set_total_tick_count(uint16_t tick_count) {
    if (m_obj)
      lv_scale_set_total_tick_count(m_obj, tick_count);
    return *this;
  }

  Scale& set_major_tick_every(uint16_t nth_tick) {
    if (m_obj)
      lv_scale_set_major_tick_every(m_obj, nth_tick);
    return *this;
  }

  Scale& set_label_show(bool show) {
    if (m_obj)
      lv_scale_set_label_show(m_obj, show);
    return *this;
  }
};
}  // namespace LVGL_Wrapper
