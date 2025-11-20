#pragma once
#include "./Color.hpp"
#include "../core/BaseWidget.hpp"
#include "../core/Widget.hpp"
extern "C" {
#include "lvgl.h"
}

namespace LVGL_Wrapper {
class Chart;  // Forward declare Chart to avoid circular dependency
namespace types {

class ChartSeries {
  friend class LVGL_Wrapper::Chart;

 private:
  lv_chart_series_t* m_series;

  explicit ChartSeries(lv_chart_series_t* series) : m_series(series) {}

 public:
  ChartSeries(const ChartSeries&) = delete;
  ChartSeries& operator=(const ChartSeries&) = delete;

  lv_chart_series_t* raw() const { return m_series; }

  // Note: This function needs the 'Chart' parent object
  // to call the LVGL API function.
  void set_color(Chart* chart, const Color& color);
};
}  // namespace types
}  // namespace LVGL_Wrapper
