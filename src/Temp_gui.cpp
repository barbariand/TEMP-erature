#include "Temp_gui.hpp"
#include "network.h"
#include "wifi_cred.h"
#include <iostream>
#include <ctime>

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
  if (m_t2 && m_t2_label) {
    apply_tile_colors(*m_t2, *m_t2_label, m_t2_dark);
    connect_wifi();
  }
}

int get_today_index()
{
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    
    // tm_wday: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    // Men du verkar använda måndag först i din UI, så vi konverterar
    int wday = t->tm_wday;    // 0–6

    // Gör måndag = 0, tisdag = 1, ... söndag = 6
    return (wday == 0) ? 6 : (wday - 1);
}

void TempGUI::create_ui() {
  auto screen = Screen::getInstance();
  m_tileview = Tileview::create(*screen);

  m_tileview

      ->set_size(screen->get_width(), screen->get_height())
      .set_scrollbar_mode(ScrollbarMode::Off);

  m_t1 = m_tileview->add_tile(0, 0, Direction::Horizontal);
  if (m_t1) {
    m_t1_label = Label::create(*m_t1);
    m_t1_label->set_text("Group 15, ver. 0.7")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*m_t1, *m_t1_label, false);
  }

  m_t2 = m_tileview->add_tile(3, 0, Direction::Horizontal);
  if (m_t2) {
    m_t2_label = Label::create(*m_t2);
    m_t2_label->set_text("Connect to wifi")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(*m_t2, *m_t2_label, false);
    m_t2->add_flag(Flag::Clickable);
    m_t2->on_clicked([this]() { this->on_tile2_clicked_member(); });
  }

  m_t3 = m_tileview->add_tile(2, 0, Direction::Horizontal);
  if (m_t3) {
    // Create chart
    lv_obj_t* chart = lv_chart_create(m_t3->raw());
    lv_obj_set_size(chart, screen->get_width() - 40, screen->get_height() - 60);
    lv_obj_center(chart);

    // Store chart so we can update later if needed
    m_chart = chart;

    // Configure chart
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, 10);

    // Add a data series
    /*lv_chart_series_t* ser = lv_chart_add_series(
        chart,
        lv_palette_main(LV_PALETTE_BLUE),
        LV_CHART_AXIS_PRIMARY_Y
    );*/
    m_series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);


    // Dummy values
    for (int i = 0; i < 10; i++) {
      lv_chart_set_next_value(chart, m_series, rand() % 100);
    }
  }

  m_t4 = m_tileview->add_tile(1, 0, Direction::Horizontal);
  if (m_t4) {
    m_t4_label = Label::create(*m_t4);
    m_t4_label->set_text("Karlskrona")
        .set_style_text_font(&lv_font_montserrat_28)
        .align(LVGL_Wrapper::Align::TopLeft,10,10);


    const char* dayNames[7] = {
      "Monday", "Tuesday", "Wednesday",
      "Thursday", "Friday", "Saturday", "Sunday"
    };

    int today = get_today_index();

    // Skapa en omroterad lista
    std::vector<std::string> ordered_days;
    ordered_days.reserve(7);

    for (int i = 0; i < 7; i++) { 
        ordered_days.push_back(dayNames[(today + i) % 7]);
    }

    for (int i = 0; i < 7; i++) {
        m_labels[i] = Label::create(*m_t4);
        m_labels[i]->set_text(ordered_days[i].c_str())
            .set_style_text_font(&lv_font_montserrat_16);

        if (i == 0) {
            // First label goes under the title
            m_labels[i]->align_to(*m_t4_label,
                LVGL_Wrapper::Align::OutBottomLeft, 0,20);
        } else {
            // The rest chain downward
            m_labels[i]->align_to(*m_labels[i - 1],
                LVGL_Wrapper::Align::OutMidRight, 10, 0);
        }
    }
    apply_tile_colors(*m_t4, *m_t4_label, false);
  }
}

void TempGUI::update_chart(int new_value) {
    if (m_chart && m_series) {
        lv_chart_set_next_value(m_chart, m_series, new_value);
    }
}
  