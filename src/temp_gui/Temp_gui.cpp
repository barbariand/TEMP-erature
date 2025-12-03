#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

#include "api/cities.hpp"
#include "api/data/DataPoint.hpp"
#include "api/data/ForcastSevenDays.hpp"
#include "api/data/ObservationSeries.hpp"
#include "api/parameters/MeterologyCode.hpp"
#include "api/parameters/SevenDayForcastParameters.hpp"
#include "api/smhi_client.hpp"
#include "api/url/SMHIUrlGenerator.hpp"
#include "lvgl.h"
#include "network/network.hpp"
#include "settings_storage.hpp"
#include "types/Enums.hpp"

using namespace LVGL_Wrapper;

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();

  m_tileview = Tileview::create(*screen);
  m_tileview->set_size(LV_PCT(100), LV_PCT(100));
  m_tileview->set_scrollbar_mode(ScrollbarMode::Off);

  m_t0_group = m_tileview->add_tile(0, 0, Direction::Right);
  m_c0_group = Component::create<MainUi>(*m_t0_group);

  m_t1_forecast = m_tileview->add_tile(1, 0, Direction::Horizontal);
  m_c1_forecast = Component::create<ForcastUI>(*m_t1_forecast);

  m_t2_chart = m_tileview->add_tile(2, 0, Direction::Horizontal);
  m_c2_chart = Component::create<HistoryChartUi>(*m_t2_chart);

  m_t3_settings = m_tileview->add_tile(3, 0, Direction::Horizontal);
  m_c3_settings = Component::create<SettingsUi>(*m_t3_settings);

  if (m_c3_settings) {
    m_c3_settings->on_save = [this](const SettingsData& s) {
      this->handle_settings_save(s);
    };
  }

  m_t4_wifi = m_tileview->add_tile(4, 0, Direction::Left);
  m_c4_wifi = Component::create<WifiUi>(*m_t4_wifi);

  Settings s = SettingsStorage::load();

  SettingsData sd;
  sd.city = s.city;
  sd.parameter = s.parameter;

  handle_settings_save(sd);
}

void TempGUI::handle_settings_save(const SettingsData& settings) {
  std::cout << "[TempGUI] Settings saved. Station ID: " << settings.city
            << std::endl;

  float lat = 0.0f;
  float lon = 0.0f;
  std::string city_name = "Unknown";
  int station_id = settings.city;
  bool found = false;

  for (const auto& c : kKnownCities) {
    if (c.station == station_id) {
      lat = c.lat;
      lon = c.lon;
      city_name = c.name;
      found = true;
      break;
    }
  }

  if (!found) {
    std::cout << "[TempGUI] Error: Station ID not found: " << station_id
              << std::endl;

    city_name = kKnownCities[0].name;
    lat = kKnownCities[0].lat;
    lon = kKnownCities[0].lon;
    station_id = kKnownCities[0].station;
  }

  if (m_c1_forecast) {
    m_c1_forecast->set_city(city_name.c_str());
  }

  fetch_forecast(lat, lon);

  fetch_history(station_id, settings.parameter);
}

void TempGUI::fetch_history(int station_id, MeterologyCode param_code) {
  StationsLatestMonthsParameters history_params;
  history_params.station = station_id;
  history_params.meterology = param_code;

  std::cout << "[TempGUI] Fetching history station: " << station_id
            << " Param Code: " << (int)param_code.value << std::endl;

  if (fetch_latest_months(history_params, m_history_data)) {
    if (m_c2_chart)

      m_c2_chart->update_data(m_history_data, param_code.toInfo().name);
  } else {
    std::cout << "[TempGUI] History fetch failed." << std::endl;
    if (m_c2_chart)
      m_c2_chart->set_error("History Fetch Error");
  }
}

void TempGUI::fetch_forecast(float lat, float lon) {
  SevenDayForcastParameters param;
  param.location.lat = lat;
  param.location.lon = lon;

  if (fetch_seven_day_forecast(param, m_forecast_data)) {
    if (m_c1_forecast)
      m_c1_forecast->update(m_forecast_data);
    std::cout << "[TempGUI] Forecast updated." << std::endl;
  } else {
    std::cout << "[TempGUI] Forecast fetch failed." << std::endl;
  }
}
