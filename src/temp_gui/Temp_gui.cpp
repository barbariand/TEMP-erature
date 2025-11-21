#include "Temp_gui.hpp"
#include <iostream>
#include <memory>
#include "GUI.hpp"
#include <ArduinoJson.h>
#include "api/api.hpp"
#include "network/network.hpp"
#include "types/Enums.hpp"
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
  m_t2_dark = !m_t2_dark;
  if (m_tile_example && m_t2_label) {
    apply_tile_colors(*m_tile_example, *m_t2_label, m_t2_dark);
    connect_wifi();
  }
}

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();
  m_tileview = Tileview::create(*screen);

  m_tileview->set_size(screen->get_width(), screen->get_height())
      .set_scrollbar_mode(ScrollbarMode::Off);

  m_tile_group = m_tileview->add_tile(0, 0, Direction::Horizontal);
  if (m_tile_group) {
    m_t1_label = Label::create(*m_tile_group);
    m_t1_label->set_text("Group 15, ver. 0.1")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*m_tile_group, *m_t1_label, false);
  }

  m_tile_example = m_tileview->add_tile(1, 0, Direction::Horizontal);
  if (m_tile_example) {
    m_t2_label = Label::create(*m_tile_example);
    m_t2_label->set_text("Connect to wifi")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*m_tile_example, *m_t2_label, false);
    m_tile_example->add_flag(Flag::Clickable);
    m_tile_example->on_clicked([this]() { this->on_tile2_clicked_member(); });
  }
  m_tile_forcast = m_tileview->add_tile(2, 0, Direction::Horizontal);
  forcast_ui = Component::create<ForcastUI>(*m_tile_forcast);

  ArduinoJson::JsonDocument doc;

  ArduinoJson::deserializeJson(doc, JSON);
  ForecastSevenDay data;
  data.fromJson(doc);
  forcast_ui->update(data);
}
