#pragma once
#include "../core/Widget.hpp"
#include "./mixins/PartIndicatorProperties.hpp"

namespace LVGL_Wrapper {

class Spinner : public Widget, public PartIndicatorProperties<Spinner> {
 private:
  uint32_t m_spin_time;
  uint32_t m_arc_angle;

  explicit Spinner(BaseWidget* parent, uint32_t time = 1000,
                   uint32_t angle = 60)
      : Widget(lv_spinner_create, parent),
        m_spin_time(time),
        m_arc_angle(angle) {
    if (m_obj)
      lv_spinner_set_anim_params(m_obj, m_spin_time, m_arc_angle);
  }

 public:
  static std::shared_ptr<Spinner> create(BaseWidget& parent,
                                         uint32_t time = 1000,
                                         uint32_t angle = 60) {
    auto new_spinner =
        std::shared_ptr<Spinner>(new Spinner(&parent, time, angle));
    parent.register_child(new_spinner);
    return new_spinner;
  }

  Spinner& set_spin_time(uint32_t time_ms) {
    m_spin_time = time_ms;
    if (m_obj)
      lv_spinner_set_anim_params(m_obj, m_spin_time, m_arc_angle);
    return *this;
  }

  Spinner& set_arc_angle(uint32_t angle_deg) {
    m_arc_angle = angle_deg;
    if (m_obj)
      lv_spinner_set_anim_params(m_obj, m_spin_time, m_arc_angle);
    return *this;
  }

  uint32_t get_spin_time() const { return m_spin_time; }
  uint32_t get_arc_angle() const { return m_arc_angle; }
};

}  // namespace LVGL_Wrapper
