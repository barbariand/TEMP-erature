#pragma once
#include <optional>
#include "../core/Widget.hpp"
#include "../types/CalendarDate.hpp"

namespace LVGL_Wrapper {

class Calendar : public Widget {
 private:
  explicit Calendar(BaseWidget* parent) : Widget(lv_calendar_create, parent) {}

 public:
  static std::shared_ptr<Calendar> create(BaseWidget& parent) {
    auto new_calendar = std::shared_ptr<Calendar>(new Calendar(&parent));
    parent.register_child(new_calendar);
    return new_calendar;
  }

  Calendar& set_today_date(uint32_t year, uint32_t month, uint32_t day) {
    if (m_obj)
      lv_calendar_set_today_date(m_obj, year, month, day);
    return *this;
  }

  Calendar& set_showed_date(uint32_t year, uint32_t month) {
    if (m_obj)
      lv_calendar_set_showed_date(m_obj, year, month);
    return *this;
  }

  std::optional<types::CalendarDate> get_pressed_date() const {
    if (!m_obj)
      return std::nullopt;

    lv_calendar_date_t out;
    lv_result_t res = lv_calendar_get_pressed_date(m_obj, &out);
    if (res != LV_RESULT_OK) {
      return std::nullopt;
    }
    return types::CalendarDate{out.year, out.month, out.day};
  }
};

}  // namespace LVGL_Wrapper
