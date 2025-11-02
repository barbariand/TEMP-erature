#pragma once
#include "Tile.hpp"  // Forward declaration not enough, need full def for std::unique_ptr
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Tileview : public Widget {
  friend class Tile;

 private:
  /**
     * @brief Raw LVGL tile creation. Kept private.
     * Called by Tile::create().
     */
  lv_obj_t* add_tile_raw(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
    if (!m_obj)
      return nullptr;
    return lv_tileview_add_tile(m_obj, col_id, row_id, dir);
  }

  // Tile needs to access the private add_tile_raw
  friend class Tile;

 public:
  /**
     * @brief Default constructor for two-stage init.
     */
  Tileview() : Widget() {}

  /**
     * @brief Creates the LVGL object. Called by parent.add_child<Tileview>().
     */
  Tileview& create(BaseWidget& parent) {
    wrap(lv_tileview_create(parent.raw()));
    return *this;
  }

  /**
     * @brief Adds a new tile to the Tileview.
     *
     * This is the C++ way to add a tile. It creates, wraps,
     * and stores the C++ Tile object.
     */
  Tile& add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
    // 1. Create the C++ wrapper
    auto tile_ptr = std::make_unique<Tile>();

    // 2. Create the LVGL object using Tileview's specific method
    tile_ptr->create(*this, col_id, row_id, dir);

    // 3. Store and return
    Tile& tile_ref = *tile_ptr;
    m_children.push_back(std::move(tile_ptr));
    return tile_ref;
  }
};

}  // namespace LVGL_Wrapper
