#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include "GUI.hpp"
#include "api/api.hpp"
#include "api/cities.hpp"
#include "api/data/DataPoint.hpp"
#include "api/data/ForcastSevenDays.hpp"
#include "api/data/ObservationSeries.hpp"
#include "api/parameters/SevenDayForcastParameters.hpp"
#include "api/smhi_client.hpp"
#include "api/url/SMHIUrlGenerator.hpp"
#include "lvgl.h"
#include "network/network.hpp"
#include "settings_storage.hpp"
#include "types/Enums.hpp"

using namespace LVGL_Wrapper;

static lv_obj_t* g_xaxis_cont = nullptr;

void TempGUI::apply_tile_colors(Widget& tile, Label& label, bool dark) {
  Color bg_color = dark ? Color::Black : Color::White;
  Color text_color = dark ? Color::White : Color::Black;
  tile.set_style_bg_opa(Opa::Cover).set_style_bg_color(bg_color);
  label.set_style_text_color(text_color);
}

void TempGUI::on_tile2_clicked_member() {
  wifi_dark = !wifi_dark;
  if (wifi_tile && wifi_label) {
    apply_tile_colors(*wifi_tile, *wifi_label, wifi_dark);

    connect_wifi();
  }
}

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();
  int screen_w = screen->get_width();
  int screen_h = screen->get_height();

  tileview = Tileview::create(*screen);
  tileview->set_size(LV_PCT(100), LV_PCT(100))
      .set_scrollbar_mode(ScrollbarMode::Off);

  auto add_nav_arrows = [&](Widget& tile_widget, bool show_left,
                            bool show_right) {
    int arrow_y = (screen_h / 2) - 14;

    if (show_left) {
      auto aL = Label::create(tile_widget);
      aL->set_text(LV_SYMBOL_LEFT)
          .set_style_text_font(&lv_font_montserrat_28)
          .set_style_text_color(Color::from_lv_color(lv_color_hex(0x888888)));
      aL->set_pos(10, arrow_y);

      lv_obj_add_flag(aL->raw(), LV_OBJ_FLAG_FLOATING);
    }

    if (show_right) {
      auto aR = Label::create(tile_widget);
      aR->set_text(LV_SYMBOL_RIGHT)
          .set_style_text_font(&lv_font_montserrat_28)
          .set_style_text_color(Color::from_lv_color(lv_color_hex(0x888888)));
      aR->set_pos(screen_w - 30, arrow_y);
      lv_obj_add_flag(aR->raw(), LV_OBJ_FLAG_FLOATING);
    }
  };

  group_tile = tileview->add_tile(0, 0, Direction::Horizontal);
  if (group_tile) {
    group_label = Label::create(*group_tile);
    group_label->set_text("Group 15, ver. 0.9")
        .set_style_text_font(&lv_font_montserrat_28)
        .set_style_text_align(LV_TEXT_ALIGN_CENTER)
        .center();
    apply_tile_colors(*group_tile, *group_label, false);
    add_nav_arrows(*group_tile, false, true);
  }

  forcast_tile = tileview->add_tile(1, 0, Direction::Horizontal);
  if (forcast_tile) {
    lv_obj_set_style_pad_hor(forcast_tile->raw(), 40, 0);
    forcast_ui = Component::create<ForcastUI>(*forcast_tile);
    forcast_tile->set_style_bg_opa(Opa::Cover).set_style_bg_color(Color::White);
    add_nav_arrows(*forcast_tile, true, true);
  }

  chart_tile = tileview->add_tile(2, 0, Direction::Horizontal);
  if (chart_tile) {
    lv_obj_set_flex_flow(chart_tile->raw(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(chart_tile->raw(), LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_pad_hor(chart_tile->raw(), 45, 0);
    lv_obj_set_style_pad_top(chart_tile->raw(), 10, 0);
    lv_obj_set_style_pad_gap(chart_tile->raw(), 5, 0);

    chart_label = Label::create(*chart_tile);
    chart_label->set_text("History")
        .set_style_text_font(&lv_font_montserrat_20)
        .set_style_text_align(LV_TEXT_ALIGN_CENTER)
        .set_width(LV_PCT(100));

    chart = Chart::create(*chart_tile);
    chart->set_width(LV_PCT(100));
    chart->set_height(LV_PCT(40));
    chart->set_type(LV_CHART_TYPE_LINE).set_point_count(7);
    chart->set_style_bg_color(Color::White);
    lv_obj_set_style_size(chart->raw(), 4, 4, LV_PART_INDICATOR);

    series = chart->add_series(
        Color::from_lv_color(lv_palette_main(LV_PALETTE_BLUE)),
        LV_CHART_AXIS_PRIMARY_Y);

    g_xaxis_cont = lv_obj_create(chart_tile->raw());
    lv_obj_set_size(g_xaxis_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(g_xaxis_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(g_xaxis_cont, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(g_xaxis_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_xaxis_cont, 0, 0);
    lv_obj_set_style_pad_all(g_xaxis_cont, 0, 0);
    lv_obj_set_style_pad_hor(g_xaxis_cont, 5, 0);

    lv_obj_t* stats_cont = lv_obj_create(chart_tile->raw());
    lv_obj_set_size(stats_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(stats_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(stats_cont, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(stats_cont, lv_color_hex(0xF2F2F2), 0);
    lv_obj_set_style_radius(stats_cont, 6, 0);
    lv_obj_set_style_pad_all(stats_cont, 6, 0);
    lv_obj_set_style_border_width(stats_cont, 0, 0);

    chart_stat_min = Label::create(*chart_tile);
    chart_stat_avg = Label::create(*chart_tile);
    chart_stat_max = Label::create(*chart_tile);

    lv_obj_set_parent(chart_stat_min->raw(), stats_cont);
    lv_obj_set_parent(chart_stat_avg->raw(), stats_cont);
    lv_obj_set_parent(chart_stat_max->raw(), stats_cont);

    chart_stat_min->set_text("Min: --").set_style_text_font(
        &lv_font_montserrat_14);
    chart_stat_avg->set_text("Avg: --").set_style_text_font(
        &lv_font_montserrat_14);
    chart_stat_max->set_text("Max: --").set_style_text_font(
        &lv_font_montserrat_14);

    lv_obj_t* slider_cont = lv_obj_create(chart_tile->raw());
    lv_obj_set_size(slider_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(slider_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(slider_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(slider_cont, 0, 0);
    lv_obj_set_style_pad_all(slider_cont, 0, 0);

    chart_slider_label = Label::create(*chart_tile);
    lv_obj_set_parent(chart_slider_label->raw(), slider_cont);
    chart_slider_label->set_text("Scroll Time")
        .set_style_text_font(&lv_font_montserrat_12)
        .set_style_text_align(LV_TEXT_ALIGN_CENTER)
        .set_width(LV_PCT(100));

    chart_slider = Slider::create(*chart_tile);
    lv_obj_set_parent(chart_slider->raw(), slider_cont);
    chart_slider->set_width(LV_PCT(100));
    chart_slider->set_range(0, 0);

    chart_slider->on_value_changed([this]() {
      Settings s = SettingsStorage::load();
      this->populate_chart_for_parameter(s.parameter);
    });

    add_nav_arrows(*chart_tile, true, true);
  }

  setting_tile = tileview->add_tile(3, 0, Direction::Horizontal);
  if (setting_tile) {
    lv_obj_set_style_pad_hor(setting_tile->raw(), 40, 0);
    setting_ui = Component::create<SettingsUI>(*setting_tile);
    add_nav_arrows(*setting_tile, true, true);
  }

  wifi_tile = tileview->add_tile(4, 0, Direction::Horizontal);
  if (wifi_tile) {
    lv_obj_set_style_pad_hor(wifi_tile->raw(), 40, 0);
    wifi_label = Label::create(*wifi_tile);
    wifi_label->set_text("Connect to wifi")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();
    apply_tile_colors(*wifi_tile, *wifi_label, false);
    wifi_tile->add_flag(Flag::Clickable);
    wifi_tile->on_clicked([this]() { this->on_tile2_clicked_member(); });
    add_nav_arrows(*wifi_tile, true, false);
  }

  lv_tileview_set_tile_by_index(tileview->raw(), 0, 0, LV_ANIM_OFF);

  if (setting_ui) {
    setting_ui->on_save_callback = [this](const Settings& s) {
      std::cout << "[DEBUG] Settings Saved. City: " << s.city << std::endl;

      if (forcast_ui && !s.city.empty())
        forcast_ui->set_city(s.city.c_str());

      bool city_found = false;
      std::string search_city = s.city;

      for (const auto& c : kKnownCities) {
        if (search_city == c.name) {
          city_found = true;
          ArduinoJson::JsonDocument doc;
          bool fetched = false;

          SevenDayForcastParameters param;
          param.location.lat = c.lat;
          param.location.lon = c.lon;
          ForecastSevenDay out;
          if (fetch_seven_day_forecast(param, out)) {
            fetched = true;
            forecast_data = out;
            std::cout << "[DEBUG] API Fetch Success!" << std::endl;
          } else {
            std::cout << "[DEBUG] API FAILED! Check WiFi." << std::endl;
          }
          if (forcast_ui)
            forcast_ui->update(forecast_data);
          this->populate_chart_for_parameter(s.parameter);
          break;
        }
      }
      if (!city_found)
        std::cout << "[DEBUG] City not found in list." << std::endl;
    };
  }

  Settings s = SettingsStorage::load();
  if (!s.city.empty() && forcast_ui)
    forcast_ui->set_city(s.city.c_str());

  if (!s.city.empty()) {
    std::string search_city = s.city;
    for (const auto& c : kKnownCities) {
      if (search_city == c.name) {
        ArduinoJson::JsonDocument doc;
        bool fetched = false;
        StationsLatestMonthsParameters param;
        param.meterology = s.parameter;
        auto it = std::find(setting_ui->city_list.begin(),
                            setting_ui->city_list.end(), s.city);
        param.station =
            kKnownCities[it - setting_ui->city_list.begin()].station;
        ObservationSeries out;
        if (fetch_latest_months(param, out)) {
          fetched = true;
          observation_data = out;
          std::cout << "[DEBUG] API Fetch Success!" << std::endl;
        } else {
          std::cout << "[DEBUG] API FAILED! Check WiFi." << std::endl;
        }
        if (forcast_ui)
          forcast_ui->update(forecast_data);
        break;
      }
    }
  }
  populate_chart_for_parameter(s.parameter);
}

void TempGUI::update_chart(int new_value) {
  if (chart && series)
    chart->set_next_value(*series, new_value);
}

void TempGUI::populate_chart_for_parameter(const MeterologyCode& parameter) {

  MeterologyCodeInfo info = parameter.toInfo();

  if (chart_title)
    chart_title->set_text(info.name);
  if (chart_ylabel)
    chart_ylabel->set_text(info.unit);

  std::vector<DataPoint> all_points = observation_data.getPoints();

  if (all_points.empty()) {
    if (chart_slider) {
      chart_slider->set_range(0, 0);
      chart_slider->set_value(0, false);
    }
    if (chart && series) {

      for (int i = 0; i < 7; i++)
        chart->set_next_value(*series, LV_CHART_POINT_NONE);
    }

    if (g_xaxis_cont)
      lv_obj_clean(g_xaxis_cont);

    if (chart_stat_min)
      chart_stat_min->set_text("Min: --");
    if (chart_stat_avg)
      chart_stat_avg->set_text("Avg: --");
    if (chart_stat_max)
      chart_stat_max->set_text("Max: --");
    return;
  }

  const int view_width = 7;
  int total_points = static_cast<int>(all_points.size());
  int max_scroll_idx =
      (total_points > view_width) ? (total_points - view_width) : 0;

  if (chart_slider) {
    int current_val = chart_slider->get_value();
    int prev_max = chart_slider->get_max_value();

    chart_slider->set_range(0, max_scroll_idx);

    if (current_val == 0 && max_scroll_idx > 0) {
      current_val = max_scroll_idx;
    }

    if (current_val > max_scroll_idx)
      current_val = max_scroll_idx;
    chart_slider->set_value(current_val, false);
  }

  int start_index = (chart_slider) ? chart_slider->get_value() : 0;
  int end_index = start_index + view_width;
  if (end_index > total_points)
    end_index = total_points;

  if (g_xaxis_cont)
    lv_obj_clean(g_xaxis_cont);

  float local_min = 10000.0f;
  float local_max = -10000.0f;
  double sum = 0;
  int count = 0;
  std::vector<float> view_values;
  std::vector<SimpleDate> view_dates;

  for (int i = start_index; i < end_index; ++i) {
    const auto& p = all_points[i];
    view_values.push_back(p.value);
    view_dates.push_back(p.date);

    if (p.value < local_min)
      local_min = p.value;
    if (p.value > local_max)
      local_max = p.value;
    sum += p.value;
    count++;
  }

  if (chart && series) {

    int padding = view_width - count;
    for (int k = 0; k < padding; k++) {
      chart->set_next_value(*series, LV_CHART_POINT_NONE);
    }

    for (size_t i = 0; i < view_values.size(); i++) {

      chart->set_next_value(*series,
                            static_cast<int>(std::round(view_values[i])));

      if (g_xaxis_cont) {
        lv_obj_t* lbl = lv_label_create(g_xaxis_cont);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_28, 0);

        char date_buf[16];

        snprintf(date_buf, sizeof(date_buf), "%d/%d", view_dates[i].day,
                 view_dates[i].month);
        lv_label_set_text(lbl, date_buf);
      }
    }

    if (count > 0) {
      int range_min = static_cast<int>(std::floor(local_min)) - 2;
      int range_max = static_cast<int>(std::ceil(local_max)) + 2;
      chart->set_range(LV_CHART_AXIS_PRIMARY_Y, range_min, range_max);
    }
  }

  if (count > 0) {
    char buf[64];

    if (chart_stat_min) {
      snprintf(buf, sizeof(buf), "Min: %.1f %s", local_min, info.unit.c_str());
      chart_stat_min->set_text(buf);
    }
    if (chart_stat_max) {
      snprintf(buf, sizeof(buf), "Max: %.1f %s", local_max, info.unit.c_str());
      chart_stat_max->set_text(buf);
    }
    if (chart_stat_avg) {
      snprintf(buf, sizeof(buf), "Avg: %.1f %s", (sum / count),
               info.unit.c_str());
      chart_stat_avg->set_text(buf);
    }
  }
}
