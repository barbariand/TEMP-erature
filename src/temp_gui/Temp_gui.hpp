#pragma once
#include <GUI.hpp>
#include <memory>
#include <vector>
#include "api/api.hpp"
#include "api/data/ForcastSevenDays.hpp"
#include "api/data/ObservationSeries.hpp"
#include "api/parameters/MeterologyCode.hpp"
#include "api/smhi_client.hpp"
#include "tiles/ForcastUi.hpp"
#include "tiles/SettingsUi.hpp"
#include <atomic>

#include "tiles/ForcastUi.hpp"
#include "tiles/HistoryChartUi.hpp"
#include "tiles/MainUi.hpp"
#include "tiles/SettingsUi.hpp"
#include "tiles/WifiUi.hpp"

class TempGUI {
 private:
  std::shared_ptr<LVGL_Wrapper::Tileview> m_tileview;

  std::shared_ptr<LVGL_Wrapper::Tile> m_t0_group;
  std::shared_ptr<MainUi> m_c0_group;

  std::shared_ptr<LVGL_Wrapper::Tile> m_t1_forecast;
  std::shared_ptr<ForcastUI> m_c1_forecast;

  std::shared_ptr<LVGL_Wrapper::Tile> m_t2_chart;
  std::shared_ptr<HistoryChartUi> m_c2_chart;

  std::shared_ptr<LVGL_Wrapper::Tile> m_t3_settings;
  std::shared_ptr<SettingsUi> m_c3_settings;

  std::shared_ptr<LVGL_Wrapper::Tile> m_t4_wifi;
  std::shared_ptr<WifiUi> m_c4_wifi;

  ForecastSevenDay m_forecast_data;
  ObservationSeries m_history_data;

  void handle_settings_save(const SettingsData& settings);
  void fetch_forecast(float lat, float lon);
  void fetch_history(int station_id, MeterologyCode type);

  std::atomic<bool> m_fetchRequested{false};
  std::atomic<bool> m_fetchInProgress{false};
  float m_pendingLat{0.0f};
  float m_pendingLon{0.0f};
  int m_pendingStationId{0};
  MeterologyCode m_pendingParam{MeterologyCode::AirTemperature_Momentary};

  // NEW: task starter och entry
  void start_fetch_task();
  static void fetch_task_entry(void* pvParameters);
  static void async_update_ui(void* user_data);

 public:
  void create_ui();
  void request_fetch(float lat, float lon, int station_id,
                     MeterologyCode parameter);
};
