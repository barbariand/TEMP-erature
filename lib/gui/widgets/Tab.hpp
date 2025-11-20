#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class Tabview;

class Tab : public Widget {
  friend class Tabview;

 protected:
  explicit Tab(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
};

}  // namespace LVGL_Wrapper
