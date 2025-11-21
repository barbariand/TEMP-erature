#pragma once
#include <GUI.hpp>
#include <memory>
#include "ForcastUi.hpp"
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

  // Tile 4: Wifi
  std::shared_ptr<Tile> wifi_tile;
  std::shared_ptr<Label> wifi_label;

  bool wifi_dark = false;
};
