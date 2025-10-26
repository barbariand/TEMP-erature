#include "Tile.hpp"
#include "Tileview.hpp"

namespace LVGL_Wrapper {

Tile& Tile::create(Tileview& parent, uint8_t col_id, uint8_t row_id,
                   lv_dir_t dir) {
  // We call the parent's raw-add function and wrap the result
  wrap(parent.add_tile_raw(col_id, row_id, dir));
  return *this;
}

}  // namespace LVGL_Wrapper
