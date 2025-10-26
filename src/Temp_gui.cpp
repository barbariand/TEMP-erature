#include "Temp_gui.hpp"
#include <lvgl.h>
#include <iostream>
#include "GUI.hpp"  // Use the new Screen singleton

void MyApplication::apply_tile_colors(LVGL_Wrapper::Widget& tile,
                                      LVGL_Wrapper::Widget& label, bool dark) {
  std::cout << "lets goooooooo changing colour" << std::endl;

  lv_color_t bg_color = dark ? lv_color_black() : lv_color_white();
  lv_color_t text_color = dark ? lv_color_white() : lv_color_black();

  // Use the builder methods
  tile.set_style_bg_opa(LV_OPA_COVER).set_style_bg_color(bg_color);

  label.set_style_text_color(text_color);
}

void MyApplication::on_tile2_clicked_static(lv_event_t* e) {
  // Get the 'this' pointer for our app instance
  MyApplication* app = static_cast<MyApplication*>(lv_event_get_user_data(e));
  if (app) {
    app->on_tile2_clicked_member();
  }
}

void MyApplication::on_tile2_clicked_member() {
  m_t2_dark = !m_t2_dark;
  apply_tile_colors(m_t2, m_t2_label, m_t2_dark);
}

// --- Function: Creates UI ---
void MyApplication::create_ui() {
  // 1. Get the Screen singleton.
  // This is safe ONLY because we call create_ui() AFTER lv_init().
  LVGL_Wrapper::Screen& screen = LVGL_Wrapper::Screen::getInstance();

  lv_disp_t* disp = lv_disp_get_default();

  // 2. Fullscreen Tileview
  m_tileview.create(screen);
  m_tileview.set_size(lv_disp_get_hor_res(disp), lv_disp_get_ver_res(disp))
      .set_scrollbar_mode(LV_SCROLLBAR_MODE_OFF);

  // 3. Tile #1 (Using the new flow)
  m_t1.create(m_tileview, 0, 0, LV_DIR_HOR);
  {
    m_t1_label.create(m_t1);
    m_t1_label.set_text("Group 15, ver. 1.0")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();
    apply_tile_colors(m_t1, m_t1_label, /*dark=*/false);
  }

  // 4. Tile #2 (Using the new flow)
  m_t2.create(m_tileview, 1, 0, LV_DIR_HOR);
  {
    m_t2_label.create(m_t2);
    m_t2_label.set_text("Welcome to the workshop")
        .set_style_text_font(&lv_font_montserrat_28)
        .center();

    apply_tile_colors(m_t2, m_t2_label, /*dark=*/false);

    m_t2.add_flag(LV_OBJ_FLAG_CLICKABLE)
        .add_event_cb(MyApplication::on_tile2_clicked_static, LV_EVENT_CLICKED,
                      this);  // Pass 'this' as user_data
  }
}
