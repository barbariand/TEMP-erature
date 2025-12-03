#include "HistoryChartUi.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace LVGL_Wrapper;

HistoryChartUi::HistoryChartUi(BaseWidget* parent) : Component(parent) {}

void HistoryChartUi::init() {

  set_size(LV_PCT(100), LV_PCT(100));
  set_layout(LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_all(m_obj, 10, 0);
  set_style_bg_color(Color::White);

  m_title = Label::create(*this);
  m_title->set_text("Waiting for history...")
      .set_style_text_align(LV_TEXT_ALIGN_CENTER)
      .set_width(LV_PCT(100));

  m_chart = Chart::create(*this);
  m_chart->set_width(LV_PCT(100));


  lv_obj_set_flex_grow(m_chart->raw(), 1);

  m_chart->set_type(LV_CHART_TYPE_LINE);
  m_chart->set_point_count(0);
  m_chart->set_style_bg_color(Color::White);

  lv_obj_set_scrollbar_mode(m_chart->raw(), LV_SCROLLBAR_MODE_OFF);




  lv_obj_add_flag(m_chart->raw(), LV_OBJ_FLAG_CLICKABLE);



  lv_obj_add_flag(m_chart->raw(), LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(m_chart->raw(), LV_DIR_HOR);
  lv_obj_clear_flag(m_chart->raw(),
                    LV_OBJ_FLAG_SCROLL_CHAIN_HOR);
  lv_obj_clear_flag(m_chart->raw(),
                    LV_OBJ_FLAG_GESTURE_BUBBLE);


  lv_obj_add_event_cb(
      m_chart->raw(),
      [](lv_event_t* e) {
        auto* self = static_cast<HistoryChartUi*>(lv_event_get_user_data(e));
        if (self)
          self->handle_drag(e);
      },
      LV_EVENT_ALL, this);



  m_series = m_chart->add_series(Color::from_rgb(0, 100, 200),
                                 LV_CHART_AXIS_PRIMARY_Y);

  m_stats_cont = Component::create<Component>(*this);
  m_stats_cont->set_width(LV_PCT(100));
  m_stats_cont->set_height(LV_SIZE_CONTENT);
  m_stats_cont->set_style_bg_color(Color::from_hex(0xF0F0F0));
  m_stats_cont->set_style_pad_all(5);

  lv_obj_set_flex_flow(m_stats_cont->raw(), LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(m_stats_cont->raw(), LV_FLEX_ALIGN_SPACE_BETWEEN,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  m_lbl_min = Label::create(*m_stats_cont);
  m_lbl_avg = Label::create(*m_stats_cont);
  m_lbl_max = Label::create(*m_stats_cont);

  update_stats_ui(0, 0, 0);

  auto lbl_slide = Label::create(*this);
  lbl_slide->set_text("Scroll History")
      .set_style_text_font(&lv_font_montserrat_12);

  m_slider = Slider::create(*this);
  m_slider->set_width(LV_PCT(100));

  m_slider->set_range(0, 0);

  m_slider->on_value_changed([this]() { refresh_chart_view(); });
}


void HistoryChartUi::handle_drag(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);


  if (code == LV_EVENT_PRESSED) {
    m_drag_accum_x = 0;
    return;
  }


  if (code == LV_EVENT_PRESSING) {
    lv_indev_t* indev = lv_indev_get_act();
    if (!indev)
      return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    m_drag_accum_x += vect.x;



    float width = (float)lv_obj_get_width(m_chart->raw());
    if (width < 1.0f)
      width = 240.0f;

    float pixels_per_point = width / 30.0f;
    if (pixels_per_point < 1.0f)
      pixels_per_point = 1.0f;

    int shift = 0;

    while (m_drag_accum_x >= pixels_per_point) {
      shift--;
      m_drag_accum_x -= (int)pixels_per_point;
    }

    while (m_drag_accum_x <= -pixels_per_point) {
      shift++;
      m_drag_accum_x += (int)pixels_per_point;
    }

    if (shift != 0) {
      int current_val = m_slider->get_value();
      int max_val = lv_slider_get_max_value(m_slider->raw());
      int min_val = lv_slider_get_min_value(m_slider->raw());

      int new_val = current_val + shift;


      if (new_val < min_val)
        new_val = min_val;
      if (new_val > max_val)
        new_val = max_val;

      if (new_val != current_val) {

        m_slider->set_value(new_val, LV_ANIM_OFF);
        refresh_chart_view();
      }
    }
  }
}

void HistoryChartUi::set_error(const char* msg) {
  m_title->set_text(msg);
  m_chart->set_point_count(0);
}

void HistoryChartUi::update_data(const ObservationSeries& data,
                                 const std::string& parameter) {
  m_current_param = parameter;
  m_title->set_text((parameter + " (Last Months)").c_str());

  m_raw_values.clear();
  m_y_buffer.clear();

  for (const auto& pt : data.points) {
    float val = pt.value;
    m_raw_values.push_back(val);
    m_y_buffer.push_back(static_cast<int32_t>(std::round(val)));
  }

  if (m_raw_values.empty()) {
    m_title->set_text("No Data Available");
    return;
  }

  int window_size = 30;
  int total_points = (int)m_raw_values.size();
  int max_scroll = std::max(0, total_points - window_size);

  m_slider->set_range(0, max_scroll);
  m_slider->set_value(max_scroll, LV_ANIM_ON);

  refresh_chart_view();
}

void HistoryChartUi::refresh_chart_view() {
  if (m_raw_values.empty())
    return;

  int start_idx = m_slider->get_value();
  int window_size = 30;
  int total_points = (int)m_raw_values.size();
  int end_idx = std::min(total_points, start_idx + window_size);
  int count = end_idx - start_idx;

  if (count <= 0)
    return;

  m_chart->set_point_count(count);
  m_chart->set_ext_y_array(*m_series, &m_y_buffer[start_idx]);

  float min_v = 1000.0f, max_v = -1000.0f, sum = 0.0f;
  for (int i = start_idx; i < end_idx; ++i) {
    float v = m_raw_values[i];
    if (v < min_v)
      min_v = v;
    if (v > max_v)
      max_v = v;
    sum += v;
  }

  m_chart->set_range(LV_CHART_AXIS_PRIMARY_Y, (int)min_v - 2, (int)max_v + 2);
  m_chart->refresh();

  update_stats_ui(min_v, sum / count, max_v);
}

void HistoryChartUi::update_stats_ui(float min, float avg, float max) {
  char buf[32];
  std::string unit =
      (m_current_param == "Temperature")
          ? "C"
          : (m_current_param == "Humidity"
                 ? "%"
                 : (m_current_param == "Wind Speed" ? "m/s" : ""));

  if (m_raw_values.empty()) {
    m_lbl_min->set_text("-");
    m_lbl_avg->set_text("-");
    m_lbl_max->set_text("-");
    return;
  }

  snprintf(buf, sizeof(buf), "Min: %.1f%s", min, unit.c_str());
  m_lbl_min->set_text(buf);

  snprintf(buf, sizeof(buf), "Avg: %.1f%s", avg, unit.c_str());
  m_lbl_avg->set_text(buf);

  snprintf(buf, sizeof(buf), "Max: %.1f%s", max, unit.c_str());
  m_lbl_max->set_text(buf);
}
