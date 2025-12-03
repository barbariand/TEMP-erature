#pragma once
#include <GUI.hpp>
#include <string>
#include <vector>
#include "../../api/data/ObservationSeries.hpp"

class HistoryChartUi : public LVGL_Wrapper::Component {
 private:
  std::shared_ptr<LVGL_Wrapper::Label> m_title;
  std::shared_ptr<LVGL_Wrapper::Chart> m_chart;
  std::shared_ptr<LVGL_Wrapper::Slider> m_slider;

  std::shared_ptr<LVGL_Wrapper::Widget> m_stats_cont;
  std::shared_ptr<LVGL_Wrapper::Label> m_lbl_min;
  std::shared_ptr<LVGL_Wrapper::Label> m_lbl_avg;
  std::shared_ptr<LVGL_Wrapper::Label> m_lbl_max;

  std::shared_ptr<LVGL_Wrapper::types::ChartSeries> m_series;

  std::vector<float> m_raw_values;
  std::vector<int32_t> m_y_buffer;
  std::string m_current_param;

  int m_drag_accum_x = 0;

  void refresh_chart_view();
  void update_stats_ui(float min, float avg, float max);

 public:
  explicit HistoryChartUi(LVGL_Wrapper::BaseWidget* parent);
  void init() override;

  void handle_drag(lv_event_t* e);

  void update_data(const ObservationSeries& data, const std::string& parameter);
  void set_error(const char* msg);
};
