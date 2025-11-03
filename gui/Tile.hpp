#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Tileview;

class Tile : public Widget {
 public:
  Tile() : Widget() {}

  Tile& create(Tileview& parent, uint8_t col_id, uint8_t row_id, lv_dir_t dir);
};
}  // namespace LVGL_Wrapper
