#pragma once
#include <GUI.hpp>
#include <memory>
#include "core/Component.hpp"

#include "ForcastUi.hpp"
using namespace LVGL_Wrapper;

class TempGUI {
 public:
  TempGUI() = default;
  ~TempGUI() = default;

  TempGUI(const TempGUI&) = delete;
  TempGUI& operator=(const TempGUI&) = delete;

  void create_ui();

 private:
  void apply_tile_colors(Widget& tile, Label& label, bool dark);
  void on_tile2_clicked_member();

  std::shared_ptr<Tileview> m_tileview;
  std::shared_ptr<Label> m_t1_label;
  std::shared_ptr<Label> m_t2_label;
  std::shared_ptr<Tile> m_tile_group;
  std::shared_ptr<Tile> m_tile_example;
  std::shared_ptr<Tile> m_tile_forcast;
  std::shared_ptr<ForcastUI> forcast_ui;

  bool m_t2_dark = false;
};

class StartScreen : public Component {
  std::shared_ptr<Label> group_name;
};
