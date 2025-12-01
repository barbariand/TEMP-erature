#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <iostream>
#include <memory>
#include "GUI.hpp"
#include "api/api.hpp"
#include "network/network.hpp"
#include "types/Enums.hpp"
#include "settings_storage.hpp"
#include "api/smhi_client.hpp"
#include "api/cities.hpp"
#include "lvgl.h"
#include <ctime>

const char* JSON =
#include "JSON.txt"
    ;

using namespace LVGL_Wrapper;

// Global container för X-axelns datumetiketter
static lv_obj_t* g_xaxis_cont = nullptr;

// Helper: Översätt svenska UI-namn till API-namn
std::string translate_city(const std::string& input) {
    if (input == "Göteborg") return "Gothenburg";
    if (input == "Malmö") return "Malmo";
    if (input == "Köpenhamn") return "Copenhagen";
    return input;
}

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
  }
}

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();
  int screen_w = screen->get_width();
  int screen_h = screen->get_height();

  tileview = Tileview::create(*screen);
  tileview->set_size(LV_PCT(100), LV_PCT(100))
      .set_scrollbar_mode(ScrollbarMode::Off);

  // --- PIL-LOGIK (FLOATING) ---
  auto add_nav_arrows = [&](Widget& tile_widget, bool show_left, bool show_right) {
      int arrow_y = (screen_h / 2) - 14;

      if (show_left) {
          auto aL = Label::create(tile_widget);
          aL->set_text(LV_SYMBOL_LEFT)
            .set_style_text_font(&lv_font_montserrat_28)
            .set_style_text_color(Color::from_lv_color(lv_color_hex(0x888888)));
          aL->set_pos(10, arrow_y);
          // VIKTIGT: Floating gör att de inte påverkar Flex-layouten
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

  // --- Tile 0: Start ---
  group_tile = tileview->add_tile(0, 0, Direction::Horizontal);
  if (group_tile) {
    group_label = Label::create(*group_tile);
    group_label->set_text("Group 15\nVersion 0.7")
        .set_style_text_font(&lv_font_montserrat_28)
        .set_style_text_align(LV_TEXT_ALIGN_CENTER)
        .center();
    apply_tile_colors(*group_tile, *group_label, false);
    add_nav_arrows(*group_tile, false, true);
  }

  // --- Tile 1: Forecast ---
  forcast_tile = tileview->add_tile(1, 0, Direction::Horizontal);
  if (forcast_tile) {
    lv_obj_set_style_pad_hor(forcast_tile->raw(), 40, 0); // Marginaler
    forcast_ui = Component::create<ForcastUI>(*forcast_tile);
    forcast_tile->set_style_bg_opa(Opa::Cover).set_style_bg_color(Color::White);
    add_nav_arrows(*forcast_tile, true, true);
  }

  // --- Tile 2: Chart (Historik) ---
  chart_tile = tileview->add_tile(2, 0, Direction::Horizontal);
  if (chart_tile) {
    lv_obj_set_flex_flow(chart_tile->raw(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(chart_tile->raw(), LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_pad_hor(chart_tile->raw(), 45, 0);
    lv_obj_set_style_pad_top(chart_tile->raw(), 10, 0);
    lv_obj_set_style_pad_gap(chart_tile->raw(), 5, 0);

    chart_label = Label::create(*chart_tile);
    chart_label->set_text("History")
        .set_style_text_font(&lv_font_montserrat_20)
        .set_style_text_align(LV_TEXT_ALIGN_CENTER)
        .set_width(LV_PCT(100));

    // Grafen
    chart = Chart::create(*chart_tile);
    chart->set_width(LV_PCT(100));
    chart->set_height(LV_PCT(40));
    chart->set_type(LV_CHART_TYPE_LINE).set_point_count(7);
    chart->set_style_bg_color(Color::White);
    lv_obj_set_style_size(chart->raw(), 4, 4, LV_PART_INDICATOR);

    series = chart->add_series(
        Color::from_lv_color(lv_palette_main(LV_PALETTE_BLUE)),
        LV_CHART_AXIS_PRIMARY_Y);

    // X-Axel datumrad (Mellan graf och stats)
    g_xaxis_cont = lv_obj_create(chart_tile->raw());
    lv_obj_set_size(g_xaxis_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(g_xaxis_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(g_xaxis_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(g_xaxis_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_xaxis_cont, 0, 0);
    lv_obj_set_style_pad_all(g_xaxis_cont, 0, 0);
    lv_obj_set_style_pad_hor(g_xaxis_cont, 5, 0);

    // Statistik Panel
    lv_obj_t* stats_cont = lv_obj_create(chart_tile->raw());
    lv_obj_set_size(stats_cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(stats_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(stats_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(stats_cont, lv_color_hex(0xF2F2F2), 0);
    lv_obj_set_style_radius(stats_cont, 6, 0);
    lv_obj_set_style_pad_all(stats_cont, 6, 0);
    lv_obj_set_style_border_width(stats_cont, 0, 0);

    chart_stat_min = Label::create(*chart_tile);
    chart_stat_avg = Label::create(*chart_tile);
    chart_stat_max = Label::create(*chart_tile);

    // Flytta labels till panelen
    lv_obj_set_parent(chart_stat_min->raw(), stats_cont);
    lv_obj_set_parent(chart_stat_avg->raw(), stats_cont);
    lv_obj_set_parent(chart_stat_max->raw(), stats_cont);

    chart_stat_min->set_text("Min: --").set_style_text_font(&lv_font_montserrat_14);
    chart_stat_avg->set_text("Avg: --").set_style_text_font(&lv_font_montserrat_14);
    chart_stat_max->set_text("Max: --").set_style_text_font(&lv_font_montserrat_14);

    // Slider
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

  // --- Tile 3: Settings ---
  setting_tile = tileview->add_tile(3, 0, Direction::Horizontal);
  if (setting_tile) {
    lv_obj_set_style_pad_hor(setting_tile->raw(), 40, 0);
    setting_ui = Component::create<SettingsUI>(*setting_tile);
    add_nav_arrows(*setting_tile, true, true);
  }

  // --- Tile 4: Wifi ---
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

  // --- SAVE CALLBACK (Hämta ny data) ---
  if (setting_ui) {
    setting_ui->on_save_callback = [this](const Settings& s) {
      std::cout << "[DEBUG] Settings Saved. City: " << s.city << std::endl;

      if (forcast_ui && !s.city.empty()) forcast_ui->set_city(s.city.c_str());

      bool city_found = false;
      std::string search_city = translate_city(s.city);

      for (const auto& c : kKnownCities) {
        if (search_city == c.name) {
          city_found = true;
          // Använd DynamicJsonDocument för API-kompatibilitet
          // Storlek 80KB räcker för 7-dagar, är stabilt.
          ArduinoJson::DynamicJsonDocument doc(80000);
          bool fetched = false;

          // Använd fetch_seven_day_forecast för stabilitet
          if (fetch_seven_day_forecast(c.lon, c.lat, doc)) {
             fetched = true;
             std::cout << "[DEBUG] API Fetch Success!" << std::endl;
          } else {
             std::cout << "[DEBUG] API FAILED! Check WiFi." << std::endl;
          }

          if (fetched) {
            ForecastSevenDay data;
            data.fromJson(doc);
            forecast_data = data;
            if (forcast_ui) forcast_ui->update(data);
            this->populate_chart_for_parameter(s.parameter);
          }
          break;
        }
      }
      if(!city_found) std::cout << "[DEBUG] City not found in list." << std::endl;
    };
  }

  // --- STARTUP (Försök hämta data) ---
  Settings s = SettingsStorage::load();
  if (!s.city.empty() && forcast_ui) forcast_ui->set_city(s.city.c_str());

  // Ladda cache först (snabbt)
  ArduinoJson::DynamicJsonDocument doc(80000);
  if (!ArduinoJson::deserializeJson(doc, JSON)) {
      ForecastSevenDay data;
      data.fromJson(doc);
      forecast_data = data;
      if (forcast_ui) forcast_ui->update(data);
  }

  // Försök hämta live-data om stad är vald
  if (!s.city.empty()) {
      std::string search_city = translate_city(s.city);
      for (const auto& c : kKnownCities) {
          if (search_city == c.name) {
              std::cout << "[STARTUP] Auto-fetching for: " << c.name << std::endl;
              ArduinoJson::DynamicJsonDocument fresh(80000);
              if (fetch_seven_day_forecast(c.lon, c.lat, fresh)) {
                  ForecastSevenDay data;
                  data.fromJson(fresh);
                  forecast_data = data;
                  if (forcast_ui) forcast_ui->update(data);
                  std::cout << "[STARTUP] Live data loaded!" << std::endl;
              }
              break;
          }
      }
  }

  populate_chart_for_parameter(s.parameter);
}

void TempGUI::update_chart(int new_value) {
  if (chart && series) chart->set_next_value(*series, new_value);
}

void TempGUI::populate_chart_for_parameter(const std::string& parameter) {
  if (!chart || !series) return;

  std::vector<TimeSeriesItem> points = forecast_data.get_all_mid_day_reports();
  std::vector<int32_t> xvals;
  std::vector<float> yvals_flt;
  std::vector<int32_t> yvals_int;

  // Läs data
  if (points.size() >= 1) {
    size_t take = std::min<size_t>(7, points.size());
    size_t start_idx = (points.size() > take) ? (points.size() - take) : 0;
    for (size_t i = start_idx; i < points.size(); ++i) {
        const auto& it = points[i];
        std::tm tm{};
        tm.tm_year = it.time.year - 1900;
        tm.tm_mon = it.time.month - 1;
        tm.tm_mday = it.time.day;
        tm.tm_hour = it.time.hour;
        std::time_t t = std::mktime(&tm);
        int32_t days = static_cast<int32_t>(t / 86400);

        xvals.push_back(days);
        float v = (parameter == "Temperature") ? it.data.airTemperature :
                  (parameter == "Humidity") ? static_cast<float>(it.data.relativeHumidity) :
                  (parameter == "Wind Speed") ? it.data.windSpeed : it.data.airTemperature;
        yvals_flt.push_back(v);
        yvals_int.push_back(static_cast<int32_t>(round(v)));
    }
  } else {
    // Fallback om dygnsdata saknas
    std::map<int32_t, std::vector<float>> day_map;
    for (const auto& it : forecast_data.timeSeries) {
        std::tm tm{};
        tm.tm_year = it.time.year - 1900;
        tm.tm_mon = it.time.month - 1;
        tm.tm_mday = it.time.day;
        tm.tm_hour = it.time.hour;
        std::time_t t = std::mktime(&tm);
        int32_t days = static_cast<int32_t>(t / 86400);
        float v = (parameter == "Temperature") ? it.data.airTemperature :
                  (parameter == "Humidity") ? static_cast<float>(it.data.relativeHumidity) :
                  (parameter == "Wind Speed") ? it.data.windSpeed : it.data.airTemperature;
        day_map[days].push_back(v);
    }
    for (const auto& kv : day_map) {
        float sum = 0;
        for (float v : kv.second) sum += v;
        float avg = sum / kv.second.size();
        xvals.push_back(kv.first);
        yvals_flt.push_back(avg);
        yvals_int.push_back(static_cast<int32_t>(round(avg)));
    }
  }

  if (xvals.empty()) return;

  // Uppdatera titel
  if (chart_label) {
      std::string t = parameter + " Overview";
      chart_label->set_text(t.c_str());
  }

  size_t windowSize = std::min<size_t>(7, xvals.size());
  int max_scroll = std::max<int>(0, (int)xvals.size() - (int)windowSize);

  if (chart_slider) {
      chart_slider->set_range(0, max_scroll);
      if (chart_slider->get_value() > max_scroll) {
          chart_slider->set_value(max_scroll, LV_ANIM_OFF);
      }
  }

  size_t start = (chart_slider) ? static_cast<size_t>(chart_slider->get_value()) : 0;
  if (start > max_scroll) start = max_scroll;

  std::vector<int32_t> view_y_int;
  std::vector<float> view_y_flt;
  std::vector<int32_t> view_x_days;

  for (size_t i = 0; i < windowSize; ++i) {
      view_y_int.push_back(yvals_int[start + i]);
      view_y_flt.push_back(yvals_flt[start + i]);
      view_x_days.push_back(xvals[start + i]);
  }

  chart->set_point_count(static_cast<uint16_t>(windowSize));

  int y_min = -20, y_max = 30;
  if (parameter == "Humidity") { y_min = 0; y_max = 100; }
  else if (parameter == "Wind Speed") { y_min = 0; y_max = 30; }
  else {
     float min_val = view_y_flt[0];
     float max_val = view_y_flt[0];
     for(float v : view_y_flt) {
         if(v < min_val) min_val = v;
         if(v > max_val) max_val = v;
     }
     y_min = static_cast<int>(min_val - 5);
     y_max = static_cast<int>(max_val + 5);
  }
  chart->set_range(LV_CHART_AXIS_PRIMARY_Y, y_min, y_max);

  chart_y_buffer = view_y_int;
  chart->set_ext_y_array(*series, chart_y_buffer.data());
  chart->refresh();

  // UPPDATERA DATUM-ETIKETTER
  if (g_xaxis_cont) {
      lv_obj_clean(g_xaxis_cont);
      for (size_t i = 0; i < windowSize; ++i) {
          lv_obj_t* label_obj = lv_label_create(g_xaxis_cont);
          lv_obj_set_style_text_font(label_obj, &lv_font_montserrat_12, 0);

          time_t t = static_cast<time_t>(view_x_days[i]) * 86400;
          struct tm *tm = localtime(&t);

          char buf[16];
          snprintf(buf, sizeof(buf), "%d/%d", tm->tm_mday, tm->tm_mon + 1);
          lv_label_set_text(label_obj, buf);
      }
  }

  // Statistik
  float min_v = view_y_flt[0], max_v = view_y_flt[0], sum_v = 0;
  for (float v : view_y_flt) {
      if (v < min_v) min_v = v;
      if (v > max_v) max_v = v;
      sum_v += v;
  }
  float avg_v = sum_v / windowSize;

  char buf[32];
  const char* unit = (parameter == "Temperature") ? "C" : (parameter == "Humidity") ? "%" : "m/s";

  snprintf(buf, sizeof(buf), "Min: %.1f%s", min_v, unit);
  if (chart_stat_min) chart_stat_min->set_text(buf);

  snprintf(buf, sizeof(buf), "Avg: %.1f%s", avg_v, unit);
  if (chart_stat_avg) chart_stat_avg->set_text(buf);

  snprintf(buf, sizeof(buf), "Max: %.1f%s", max_v, unit);
  if (chart_stat_max) chart_stat_max->set_text(buf);
}
