#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <iostream>
#include <memory>
#include "GUI.hpp"
#include "api/api.hpp"
#include "api/cities.hpp"
#include "api/smhi_client.hpp"
#include "network/network.hpp"
#include "settings_storage.hpp"
#include "types/Enums.hpp"

// Include network headers if they aren't in network.hpp

const char* JSON =
#include "JSON.txt"
    ;

using namespace LVGL_Wrapper;

void TempGUI::apply_tile_colors(Widget& tile, Label& label, bool dark) {
  std::cout << "lets goooooooo changing colour" << std::endl;

  Color bg_color = dark ? Color::Black : Color::White;
  Color text_color = dark ? Color::White : Color::Black;

  tile.set_style_bg_opa(Opa::Cover).set_style_bg_color(bg_color);
  label.set_style_text_color(text_color);
}

void TempGUI::on_tile2_clicked_member() {
  wifi_dark = !wifi_dark;
  if (wifi_tile && wifi_label) {
    apply_tile_colors(*wifi_tile, *wifi_label, wifi_dark);
    // connect_wifi();
  }
}

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();
  tileview = Tileview::create(*screen);

  tileview->set_size(screen->get_width(), screen->get_height())
      .set_scrollbar_mode(ScrollbarMode::Off);

  // --- Tile 0: Group Info ---
  group_tile = tileview->add_tile(0, 0, Direction::Horizontal);
  if (group_tile) {
    group_label = Label::create(*group_tile);
    group_label->set_text("Group 15, ver. 0.7")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*group_tile, *group_label, false);
  }

  // --- Tile 1: Forecast ---
  forcast_tile = tileview->add_tile(1, 0, Direction::Horizontal);
  if (forcast_tile) {
    forcast_ui = Component::create<ForcastUI>(*forcast_tile);
    forcast_tile->set_style_bg_opa(Opa::Cover).set_style_bg_color(Color::White);
  }

  // --- Tile 2: Chart ---
  chart_tile = tileview->add_tile(2, 0, Direction::Horizontal);
  if (chart_tile) {
    chart = Chart::create(*chart_tile);
    chart->set_size(screen->get_width() - 40, screen->get_height() - 60)
        .center();
    chart->set_type(LV_CHART_TYPE_LINE).set_point_count(10);

    series = chart->add_series(
        Color::from_lv_color(lv_palette_main(LV_PALETTE_BLUE)),
        LV_CHART_AXIS_PRIMARY_Y);

    // Dummy values
    for (int i = 0; i < 10; i++) {
      chart->set_next_value(*series, rand() % 100);
    }
    // Label to indicate which parameter is shown
    chart_label = Label::create(*chart_tile);
    chart_label->set_text("Parameter: Temperature")
        .set_style_text_font(&lv_font_montserrat_22)
        .set_width(LV_PCT(100))
        .set_y(8);
    // Slider below chart to select the time window/end index
    chart_slider = Slider::create(*chart_tile);
    chart_slider->set_width(screen->get_width() - 60)
        .set_y(screen->get_height() - 48)
        .center();
    chart_slider->set_range(0, 0);
    chart_slider->on_value_changed([this]() {
      Settings s = SettingsStorage::load();
      this->populate_chart_for_parameter(s.parameter);
    });

    // Small label to indicate slider meaning
    chart_slider_label = Label::create(*chart_tile);
    chart_slider_label->set_text("Oldest ← Slider → Latest")
        .set_style_text_font(&lv_font_montserrat_14)
        .set_width(LV_PCT(100));
    chart_slider_label->set_y(screen->get_height() - 28).center();

    // Statistic labels (min / avg / max) placed top-right
    chart_stat_min = Label::create(*chart_tile);
    chart_stat_avg = Label::create(*chart_tile);
    chart_stat_max = Label::create(*chart_tile);
    chart_stat_min->set_style_text_font(&lv_font_montserrat_14);
    chart_stat_avg->set_style_text_font(&lv_font_montserrat_14);
    chart_stat_max->set_style_text_font(&lv_font_montserrat_14);
    // Position roughly in the top-right corner of the chart area
    int stat_x = (screen->get_width() / 2) + (chart->get_width() / 2) - 140;
    chart_stat_min->set_pos(stat_x, 8);
    chart_stat_avg->set_pos(stat_x, 28);
    chart_stat_max->set_pos(stat_x, 48);
  }

  // --- Tile 3: Setting ---
  setting_tile = tileview->add_tile(3, 0, Direction::Horizontal);
  if (setting_tile) {
    setting_ui = Component::create<SettingsUI>(*setting_tile);
  }

  // --- Tile 4: Wifi Connect ---
  wifi_tile = tileview->add_tile(4, 0, Direction::Horizontal);
  if (wifi_tile) {
    wifi_label = Label::create(*wifi_tile);
    wifi_label->set_text("Connect to wifi")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*wifi_tile, *wifi_label, false);
    wifi_tile->add_flag(Flag::Clickable);
    wifi_tile->on_clicked([this]() { this->on_tile2_clicked_member(); });
  }

  // Ensure the first tile (0,0) is the active/start tile so Settings is fourth (index 3)
  lv_tileview_set_tile_by_index(tileview->raw(), 0, 0, LV_ANIM_OFF);

  // Wire settings save callback to apply immediately to Forecast and Chart
  if (setting_ui) {
    setting_ui->on_save_callback = [this](const Settings& s) {
      // Update city label immediately
      if (forcast_ui && !s.city.empty()) {
        forcast_ui->set_city(s.city.c_str());
      }
      if (chart_label) {
        std::string lbl = std::string("Parameter: ") + s.parameter;
        chart_label->set_text(lbl.c_str());
      }

      // Try to fetch remote data for the selected city and update UI/chart
      for (const auto& c : kKnownCities) {
        if (s.city == c.name) {
          ArduinoJson::DynamicJsonDocument doc(200000);
          bool fetched = false;
          if (fetch_latest_months(c.lon, c.lat, doc)) {
            ForecastSevenDay data;
            data.fromJson(doc);
            forecast_data = data;
            if (forcast_ui)
              forcast_ui->update(data);
            fetched = true;
          } else if (fetch_seven_day_forecast(c.lon, c.lat, doc)) {
            ForecastSevenDay data;
            data.fromJson(doc);
            forecast_data = data;
            if (forcast_ui)
              forcast_ui->update(data);
            fetched = true;
          }

          // Populate chart using selected parameter regardless (use local fallback data if fetch failed)
          this->populate_chart_for_parameter(s.parameter);
          break;
        }
      }
    };
  }

  // Load settings and apply to forecast UI (city label) and chart label
  {
    Settings s = SettingsStorage::load();
    if (!s.city.empty() && forcast_ui) {
      forcast_ui->set_city(s.city.c_str());
    }
    if (chart_label) {
      std::string lbl = std::string("Parameter: ") + s.parameter;
      chart_label->set_text(lbl.c_str());
    }
  }

  // --- Load Data ---
  ArduinoJson::JsonDocument doc;
  ArduinoJson::DeserializationError error =
      ArduinoJson::deserializeJson(doc, JSON);

  if (!error) {
    ForecastSevenDay data;
    data.fromJson(doc);
    forecast_data = data;
    if (forcast_ui)
      forcast_ui->update(data);
  } else {
    std::cout << "JSON Deserialize failed: " << error.c_str() << std::endl;
  }

  // Populate initial chart using saved parameter
  Settings s = SettingsStorage::load();
  populate_chart_for_parameter(s.parameter);
}

void TempGUI::update_chart(int new_value) {
  if (chart && series) {
    chart->set_next_value(*series, new_value);
  }
}

void TempGUI::populate_chart_for_parameter(const std::string& parameter) {
  if (!chart || !series)
    return;

  // Get midday reports first
  std::vector<TimeSeriesItem> points = forecast_data.get_all_mid_day_reports();
  // If no midday points, aggregate daily averages from full series
  std::vector<int32_t> xvals;
  std::vector<int32_t> yvals_int;
  std::vector<float> yvals_flt;  // high-precision values for display and stats

  if (points.size() >= 1) {
    // Use up to 7 last midday points
    size_t take = std::min<size_t>(7, points.size());
    size_t start = (points.size() > take) ? (points.size() - take) : 0;
    for (size_t i = start; i < points.size(); ++i) {
      const auto& it = points[i];
      std::tm tm{};
      tm.tm_year = it.time.year - 1900;
      tm.tm_mon = it.time.month - 1;
      tm.tm_mday = it.time.day;
      tm.tm_hour = it.time.hour;
      tm.tm_min = it.time.minute;
      tm.tm_sec = 0;
      tm.tm_isdst = -1;
      std::time_t t = std::mktime(&tm);
      int32_t days = static_cast<int32_t>(t / 86400);
      xvals.push_back(days);
      float vflt = (parameter == "Temperature") ? it.data.airTemperature
                   : (parameter == "Humidity")
                       ? static_cast<float>(it.data.relativeHumidity)
                   : (parameter == "Wind Speed") ? it.data.windSpeed
                                                 : it.data.airTemperature;
      yvals_flt.push_back(vflt);
      yvals_int.push_back(static_cast<int32_t>(round(vflt)));
    }
  } else {
    // Aggregate by day from forecast_data.timeSeries
    std::map<int32_t, std::vector<int32_t>> day_map;
    for (const auto& it : forecast_data.timeSeries) {
      std::tm tm{};
      tm.tm_year = it.time.year - 1900;
      tm.tm_mon = it.time.month - 1;
      tm.tm_mday = it.time.day;
      tm.tm_hour = it.time.hour;
      tm.tm_min = it.time.minute;
      tm.tm_sec = 0;
      tm.tm_isdst = -1;
      std::time_t t = std::mktime(&tm);
      int32_t days = static_cast<int32_t>(t / 86400);
      float vflt = (parameter == "Temperature") ? it.data.airTemperature
                   : (parameter == "Humidity")
                       ? static_cast<float>(it.data.relativeHumidity)
                   : (parameter == "Wind Speed") ? it.data.windSpeed
                                                 : it.data.airTemperature;
      day_map[days].push_back(static_cast<int32_t>(round(vflt)));
      // store float values in a temp map as well by summing; we'll convert to float average below
      // Using int vector for day_map for memory simplicity; compute average as float
    }
    // convert to vectors (last up to 7 days)
    for (const auto& kv : day_map) {
      int32_t day = kv.first;
      const auto& vec = kv.second;
      int32_t sum = 0;
      for (int v : vec)
        sum += v;
      float favg = vec.empty() ? 0.0f : (static_cast<float>(sum) / vec.size());
      xvals.push_back(day);
      yvals_flt.push_back(favg);
      yvals_int.push_back(static_cast<int32_t>(round(favg)));
    }
    // keep only last 7
    if (xvals.size() > 7) {
      size_t start = xvals.size() - 7;
      xvals = std::vector<int32_t>(xvals.begin() + start, xvals.end());
      yvals_flt =
          std::vector<float>(yvals_flt.begin() + start, yvals_flt.end());
      yvals_int =
          std::vector<int32_t>(yvals_int.begin() + start, yvals_int.end());
    }
  }

  if (xvals.empty() || yvals_int.empty())
    return;

  size_t windowSize = std::min<size_t>(7, xvals.size());
  chart->set_point_count(static_cast<uint16_t>(windowSize));
  chart->set_type(LV_CHART_TYPE_LINE);

  // Update Y range heuristically
  if (parameter == "Temperature")
    chart->set_range(LV_CHART_AXIS_PRIMARY_Y, -30, 40);
  else if (parameter == "Humidity")
    chart->set_range(LV_CHART_AXIS_PRIMARY_Y, 0, 100);
  else if (parameter == "Wind Speed")
    chart->set_range(LV_CHART_AXIS_PRIMARY_Y, 0, 30);

  // Use last `windowSize` entries
  std::vector<int32_t> xv(windowSize), yv(windowSize);
  std::vector<float> yvf(windowSize);
  size_t start = xvals.size() > windowSize ? xvals.size() - windowSize : 0;
  for (size_t i = 0; i < windowSize; ++i) {
    xv[i] = xvals[start + i];
    yv[i] = yvals_int[start + i];
    yvf[i] = yvals_flt[start + i];
  }

  // Bind Y values via external array (chart will read integer values directly)
  chart_y_buffer = yv;
  chart->set_ext_y_array(*series, chart_y_buffer.data());
  chart->refresh();

  // Remove old numeric labels
  for (auto& l : chart_x_labels)
    l.reset();
  chart_x_labels.clear();

  // Create numeric labels above each plotted point, formatted per-parameter
  for (size_t i = 0; i < windowSize; ++i) {
    lv_point_t pos;
    lv_chart_get_point_pos_by_id(chart->raw(), series->raw(),
                                 static_cast<uint32_t>(i), &pos);
    char buf[32];
    if (parameter == "Temperature") {
      snprintf(buf, sizeof(buf), "%.1f°C", yvf[i]);
    } else if (parameter == "Humidity") {
      snprintf(buf, sizeof(buf), "%d%%", static_cast<int>(round(yvf[i])));
    } else if (parameter == "Wind Speed") {
      snprintf(buf, sizeof(buf), "%.1fm/s", yvf[i]);
    } else {
      snprintf(buf, sizeof(buf), "%.1f", yvf[i]);
    }
    auto lbl = Label::create(*chart_tile);
    lbl->set_text(buf).set_style_text_font(&lv_font_montserrat_14);
    int chart_x = lv_obj_get_x(chart->raw());
    int chart_y = lv_obj_get_y(chart->raw());
    int label_w = lbl->get_width();
    int lx = chart_x + pos.x - (label_w / 2);
    int ly = chart_y + pos.y - 18 -
             static_cast<int>(i % 2) * 10;  // stagger to reduce overlap
    lbl->set_pos(lx, ly);
    chart_x_labels.push_back(lbl);
  }

  // Compute stats (min, avg, max) from yvf and display in stat labels
  float minv = yvf[0], maxv = yvf[0], sumv = 0.0f;
  for (size_t i = 0; i < windowSize; ++i) {
    minv = std::min(minv, yvf[i]);
    maxv = std::max(maxv, yvf[i]);
    sumv += yvf[i];
  }
  float avgv = sumv / static_cast<float>(windowSize);
  char minbuf[32], avgbuf[32], maxbuf[32];
  if (parameter == "Temperature") {
    snprintf(minbuf, sizeof(minbuf), "Min: %.1f°C", minv);
    snprintf(avgbuf, sizeof(avgbuf), "Avg: %.1f°C", avgv);
    snprintf(maxbuf, sizeof(maxbuf), "Max: %.1f°C", maxv);
  } else if (parameter == "Humidity") {
    snprintf(minbuf, sizeof(minbuf), "Min: %d%%",
             static_cast<int>(round(minv)));
    snprintf(avgbuf, sizeof(avgbuf), "Avg: %d%%",
             static_cast<int>(round(avgv)));
    snprintf(maxbuf, sizeof(maxbuf), "Max: %d%%",
             static_cast<int>(round(maxv)));
  } else if (parameter == "Wind Speed") {
    snprintf(minbuf, sizeof(minbuf), "Min: %.1fm/s", minv);
    snprintf(avgbuf, sizeof(avgbuf), "Avg: %.1fm/s", avgv);
    snprintf(maxbuf, sizeof(maxbuf), "Max: %.1fm/s", maxv);
  } else {
    snprintf(minbuf, sizeof(minbuf), "Min: %.1f", minv);
    snprintf(avgbuf, sizeof(avgbuf), "Avg: %.1f", avgv);
    snprintf(maxbuf, sizeof(maxbuf), "Max: %.1f", maxv);
  }
  if (chart_stat_min)
    chart_stat_min->set_text(minbuf);
  if (chart_stat_avg)
    chart_stat_avg->set_text(avgbuf);
  if (chart_stat_max)
    chart_stat_max->set_text(maxbuf);

  // Update slider range and default to latest
  if (chart_slider) {
    chart_slider->set_range(0, static_cast<int32_t>(windowSize - 1));
    chart_slider->set_value(static_cast<int>(windowSize - 1), LV_ANIM_OFF);
  }
}
