#pragma once
#include <memory>
#include "../core/Widget.hpp"
#include "../types/ChartSeries.hpp"
#include "../types/Color.hpp"
#include "../types/Enums.hpp"

namespace LVGL_Wrapper {

class Chart : public Widget {
 private:
  explicit Chart(BaseWidget* parent) : Widget(lv_chart_create, parent) {}

  std::vector<std::shared_ptr<types::ChartSeries>> m_series_storage;

 public:
  static std::shared_ptr<Chart> create(BaseWidget& parent) {
    auto new_chart = std::shared_ptr<Chart>(new Chart(&parent));
    parent.register_child(new_chart);
    return new_chart;
  }

  Chart& set_type(lv_chart_type_t type) {
    if (m_obj)
      lv_chart_set_type(m_obj, type);
    return *this;
  }

  Chart& set_point_count(uint16_t cnt) {
    if (m_obj)
      lv_chart_set_point_count(m_obj, cnt);
    return *this;
  }

  Chart& set_range(lv_chart_axis_t axis, int32_t min, int32_t max) {
    if (m_obj)
      lv_chart_set_range(m_obj, axis, min, max);
    return *this;
  }

  Chart& set_update_mode(lv_chart_update_mode_t mode) {
    if (m_obj)
      lv_chart_set_update_mode(m_obj, mode);
    return *this;
  }

  std::shared_ptr<types::ChartSeries> add_series(const Color& color,
                                                 lv_chart_axis_t axis) {
    if (!m_obj)
      return nullptr;

    lv_chart_series_t* c_series = lv_chart_add_series(m_obj, color, axis);
    if (!c_series)
      return nullptr;

    // Use a custom deleter to just release the pointer (LVGL manages it)
    auto series_ptr =
        std::shared_ptr<types::ChartSeries>(new types::ChartSeries(c_series));

    m_series_storage.push_back(series_ptr);
    return series_ptr;
  }

  Chart& set_next_value(types::ChartSeries& series, int32_t value) {
    if (m_obj)
      lv_chart_set_next_value(m_obj, series.raw(), value);
    return *this;
  }

  Chart& set_ext_y_array(types::ChartSeries& series, int32_t* array) {
    if (m_obj)
      lv_chart_set_ext_y_array(m_obj, series.raw(), array);
    return *this;
  }

  Chart& set_all_value(types::ChartSeries& series, int32_t value) {
    if (m_obj)
      lv_chart_set_all_value(m_obj, series.raw(), value);
    return *this;
  }

  Chart& refresh() {
    if (m_obj)
      lv_chart_refresh(m_obj);
    return *this;
  }
};

// Implementation for ChartSeries::set_color
// We must define it here, after Chart is fully defined.
namespace types {
inline void ChartSeries::set_color(Chart* chart, const Color& color) {
  lv_obj_t* obj = static_cast<BaseWidget*>(chart)->raw();
  if (m_series) {
    lv_chart_set_series_color(obj, m_series, color);
  }
}
}  // namespace types

}  // namespace LVGL_Wrapper
