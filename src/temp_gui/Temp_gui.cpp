#include "Temp_gui.hpp"
#include <ArduinoJson.h>
#include <iostream>
#include <memory>
#include "GUI.hpp"
#include "api/api.hpp"
#include "network/network.hpp"
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
  }

  // --- Tile 3: Wifi Connect ---
  wifi_tile = tileview->add_tile(3, 0, Direction::Horizontal);
  if (wifi_tile) {
    wifi_label = Label::create(*wifi_tile);
    wifi_label->set_text("Connect to wifi")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*wifi_tile, *wifi_label, false);
    wifi_tile->add_flag(Flag::Clickable);
    wifi_tile->on_clicked([this]() { this->on_tile2_clicked_member(); });
  }

  // --- Load Data ---
  ArduinoJson::JsonDocument doc;
  ArduinoJson::DeserializationError error =
      ArduinoJson::deserializeJson(doc, JSON);

  if (!error) {
    ForecastSevenDay data;
    data.fromJson(doc);
    forcast_ui->update(data);
  } else {
    std::cout << "JSON Deserialize failed: " << error.c_str() << std::endl;
  }
}

void TempGUI::update_chart(int new_value) {
  if (chart && series) {
    chart->set_next_value(*series, new_value);
  }
}
