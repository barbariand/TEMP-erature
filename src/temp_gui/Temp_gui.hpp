#pragma once
#include <GUI.hpp>
#include <memory>
#include "ForcastUi.hpp"
#include "api/api.hpp"
#include <vector>
#include "SettingsUi.hpp"
#include "core/Component.hpp"

using namespace LVGL_Wrapper;

class TempGUI {
 public:
  TempGUI() = default;
  ~TempGUI() = default;

  TempGUI(const TempGUI&) = delete;
  TempGUI& operator=(const TempGUI&) = delete;

  void create_ui();
  void update_chart(int new_value);

 private:
  void apply_tile_colors(Widget& tile, Label& label, bool dark);
  void on_tile2_clicked_member();
std::shared_ptr<Tileview> tileview;

  // Tile 1: Group Info
  std::shared_ptr<Tile> group_tile;
  std::shared_ptr<Label> group_label;

  // Tile 2: Forecast (Container for ForcastUI)
  std::shared_ptr<Tile> forcast_tile;
  std::shared_ptr<ForcastUI> forcast_ui;

  // Tile 3: Chart
  std::shared_ptr<Tile> chart_tile;
  std::shared_ptr<Chart> chart;
  std::shared_ptr<types::ChartSeries> series;
  std::shared_ptr<Label> chart_label;
  std::shared_ptr<Slider> chart_slider;
  std::shared_ptr<Label> chart_slider_label;
  // Labels under the chart for X-axis (dates / numeric labels)
  std::vector<std::shared_ptr<Label>> chart_x_labels;
  // Tick labels below the chart (start / mid / end)
  std::vector<std::shared_ptr<Label>> chart_tick_labels;
  // Chart title and y-axis label
  std::shared_ptr<Label> chart_title;
  std::shared_ptr<Label> chart_ylabel;
  // Statistic labels (min / avg / max)
  std::shared_ptr<Label> chart_stat_min;
  std::shared_ptr<Label> chart_stat_avg;
  std::shared_ptr<Label> chart_stat_max;

  // Store the last loaded forecast data so we can repopulate the chart
  ForecastSevenDay forecast_data;
  // External Y buffer used when binding values to LVGL chart via ext_y_array
  std::vector<int32_t> chart_y_buffer;

  // Tile 4: Wifi
  std::shared_ptr<Tile> wifi_tile;
  std::shared_ptr<Label> wifi_label;

  // Tile 5: Setting screen
  std::shared_ptr<Tile> setting_tile;
  std::shared_ptr<SettingsUI> setting_ui;

  bool wifi_dark = false;
  // Populate chart using the named parameter (e.g. "Temperature", "Humidity", "Wind Speed")
  void populate_chart_for_parameter(const std::string& parameter);
};
