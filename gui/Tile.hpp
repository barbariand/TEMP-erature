#pragma once
#include "Widget.hpp"

namespace LVGL_Wrapper {

// Forward-declare Tileview to break include cycle
class Tileview;

class Tile : public Widget {
 public:
  Tile() : Widget() {}

  /**
     * @brief Creates the tile by adding it to a parent Tileview.
     * This is called by Tileview::add_tile().
     */
  Tile& create(Tileview& parent, uint8_t col_id, uint8_t row_id, lv_dir_t dir);
};
}  // namespace LVGL_Wrapper
