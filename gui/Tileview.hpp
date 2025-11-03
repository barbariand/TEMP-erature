#pragma once
#include "Tile.hpp"
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Tileview : public Widget {
  friend class Tile;

 private:
  lv_obj_t* add_tile_raw(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
    if (!m_obj)
      return nullptr;
    return lv_tileview_add_tile(m_obj, col_id, row_id, dir);
  }

  friend class Tile;

 public:
  Tileview() : Widget() {}

  Tileview& create(BaseWidget& parent) {
    wrap(lv_tileview_create(parent.raw()));
    return *this;
  }

  Tile& add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
    auto tile_ptr = std::make_unique<Tile>();
    tile_ptr->create(*this, col_id, row_id, dir);
    Tile& tile_ref = *tile_ptr;
    m_children.push_back(std::move(tile_ptr));
    return tile_ref;
  }
};

}  // namespace LVGL_Wrapper
