#pragma once
#include "../core/Widget.hpp"
#include "../types/Enums.hpp"
#include "./Tile.hpp"
#include <map>

namespace LVGL_Wrapper {

class Tileview : public Widget {
 private:
  explicit Tileview(BaseWidget* parent) : Widget(lv_tileview_create, parent) {}
  
  std::map<std::pair<uint8_t, uint8_t>, std::shared_ptr<Tile>> m_tiles;

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
    m_tiles[{col, row}] = tile_ptr;

    return tile_ptr;
  }

  void go_to(uint8_t col, uint8_t row, bool animated = false) {
    if (!m_obj) {
      return;
    }
    
    auto it = m_tiles.find({col, row});
    if (it != m_tiles.end() && it->second && it->second->raw()) {
      lv_tileview_set_tile(m_obj, it->second->raw(), animated ? LV_ANIM_ON : LV_ANIM_OFF);
    }
  }
};

}  // namespace LVGL_Wrapper
