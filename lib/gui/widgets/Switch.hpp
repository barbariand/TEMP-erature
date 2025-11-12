#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Switch : public Widget, public PartIndicatorProperties<Switch> {
 private:
  explicit Switch(BaseWidget* parent) : Widget(lv_switch_create, parent) {}

 public:
  static std::shared_ptr<Switch> create(BaseWidget& parent) {
    auto new_switch = std::shared_ptr<Switch>(new Switch(&parent));
    parent.register_child(new_switch);
    return new_switch;
  }

  bool is_checked() const {
    return m_obj ? lv_obj_has_state(m_obj, LV_STATE_CHECKED) : false;
  }
};

}  // namespace LVGL_Wrapper
