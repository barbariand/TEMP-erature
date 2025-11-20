#pragma once
#include "../core/Widget.hpp"
#include "../types/Enums.hpp"
#include "./Tile.hpp"

namespace LVGL_Wrapper {

class Tileview : public Widget {
 private:
  explicit Tileview(BaseWidget* parent) : Widget(lv_tileview_create, parent) {}

 public:
  static std::shared_ptr<Tileview> create(BaseWidget& parent) {
    auto new_tileview = std::shared_ptr<Tileview>(new Tileview(&parent));
    parent.register_child(new_tileview);
    return new_tileview;
  }

  std::shared_ptr<Tile> add_tile(uint8_t col, uint8_t row,
                                 Direction::Type dir) {
    if (!m_obj) {
      return nullptr;
    }

    lv_obj_t* c_tile =
        lv_tileview_add_tile(m_obj, col, row, static_cast<lv_dir_t>(dir));
    if (!c_tile) {
      return nullptr;
    }

    auto tile_ptr = std::shared_ptr<Tile>(new Tile(this, c_tile));

    this->register_child(tile_ptr);

    return tile_ptr;
  }
};

}  // namespace LVGL_Wrapper
