#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Spinbox : public Widget {
 private:
  explicit Spinbox(BaseWidget* parent) : Widget(lv_spinbox_create, parent) {}

 public:
  static std::shared_ptr<Spinbox> create(BaseWidget& parent) {
    auto new_spinbox = std::shared_ptr<Spinbox>(new Spinbox(&parent));
    parent.register_child(new_spinbox);
    return new_spinbox;
  }

  Spinbox& set_range(int32_t min, int32_t max) {
    if (m_obj)
      lv_spinbox_set_range(m_obj, min, max);
    return *this;
  }

  Spinbox& set_value(int32_t value) {
    if (m_obj)
      lv_spinbox_set_value(m_obj, value);
    return *this;
  }

  int32_t get_value() const { return m_obj ? lv_spinbox_get_value(m_obj) : 0; }

  Spinbox& set_step(uint32_t step) {
    if (m_obj)
      lv_spinbox_set_step(m_obj, step);
    return *this;
  }

  Spinbox& step_next() {
    if (m_obj)
      lv_spinbox_step_next(m_obj);
    return *this;
  }

  Spinbox& step_prev() {
    if (m_obj)
      lv_spinbox_step_prev(m_obj);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
