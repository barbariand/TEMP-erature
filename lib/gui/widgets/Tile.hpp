#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Tileview;

class Tile : public Widget {
  friend class Tileview;

 protected:
  Tile(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
};

}  // namespace LVGL_Wrapper
