#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {
class Tileview;
}

namespace LVGL_Wrapper {

class Tile : public Widget {
 public:
  Tile() : Widget(nullptr) {}

  /**
   * @brief Creates the tile by adding it to a parent Tileview.
   * This is the new, cleaner flow you wanted.
   */
  Tile& create(Tileview& parent, uint8_t col_id, uint8_t row_id, lv_dir_t dir);
};

}  // namespace LVGL_Wrapper
