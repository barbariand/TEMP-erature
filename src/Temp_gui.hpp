#pragma once
#include <GUI.hpp>
#include <memory>
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

  std::shared_ptr<Tileview> m_tileview;
  std::shared_ptr<Label> m_t1_label;
  std::shared_ptr<Label> m_t2_label;
  std::shared_ptr<Label> m_t4_label;
  std::vector<std::shared_ptr<Label>> m_labels{7};
  std::shared_ptr<Tile> m_t1;
  std::shared_ptr<Tile> m_t4;
  std::shared_ptr<Tile> m_t2;
  std::shared_ptr<Tile> m_t3;
  lv_obj_t* m_chart = nullptr;
  lv_chart_series_t* m_series = nullptr;
  bool m_t2_dark = false;
};

class StartScreen : public Component {
  std::shared_ptr<Label> group_name;
};
