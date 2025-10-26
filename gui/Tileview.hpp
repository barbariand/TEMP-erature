#pragma once
#include "Tile.hpp"
#include "Widget.hpp"

namespace LVGL_Wrapper {

class Tileview : public Widget {
 public:
  /**
   * @brief Default constructor for two-stage init.
   */
  Tileview() : Widget(nullptr) {}

  /**
   * @brief Creates a new Tileview immediately.
   */
  explicit Tileview(BaseWidget& parent)
      : Widget(lv_tileview_create(parent.raw())) {}

  /**
   * @brief Creates the LVGL object. Used for two-stage init.
   */
  Tileview& create(BaseWidget& parent) {
    wrap(lv_tileview_create(parent.raw()));
    return *this;
  }

  /**
   * @brief Adds a new tile to the Tileview and returns the raw C pointer.
   * * The caller is responsible for wrapping this pointer in a Tile object.
   */
  lv_obj_t* add_tile_raw(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
    if (!m_obj)
      return nullptr;
    return lv_tileview_add_tile(m_obj, col_id, row_id, dir);
  }
};

}  // namespace LVGL_Wrapper
