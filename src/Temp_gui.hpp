#pragma once
#include <GUI.hpp>

class MyApplication {
 public:
  MyApplication() = default; // All members are default-constructed
  ~MyApplication() = default;

  MyApplication(const MyApplication&) = delete;
  MyApplication& operator=(const MyApplication&) = delete;

  /**
   * @brief Call this *after* lv_init() to build the UI.
   */
  void create_ui();

 private:
  void apply_tile_colors(LVGL_Wrapper::Widget& tile, LVGL_Wrapper::Widget& label, bool dark);
  static void on_tile2_clicked_static(lv_event_t* e);
  void on_tile2_clicked_member();

  // All wrapper objects are members (no heap).
  // They are "empty" until create_ui() is called.
  LVGL_Wrapper::Tileview m_tileview;
  LVGL_Wrapper::Label m_t1_label;
  LVGL_Wrapper::Label m_t2_label;
  LVGL_Wrapper::Tile m_t1;
  LVGL_Wrapper::Tile m_t2;

  bool m_t2_dark = false;
};
