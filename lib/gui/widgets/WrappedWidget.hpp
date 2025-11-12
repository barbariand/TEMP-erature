#pragma once
#include "../core/Widget.hpp"

namespace LVGL_Wrapper {

class WrappedWidget : public Widget {
  friend class MessageBox;
  friend class Window;

 private:
  explicit WrappedWidget(lv_obj_t* obj_to_wrap) : Widget(obj_to_wrap) {}

  WrappedWidget(lv_obj_t* obj_to_wrap, BaseWidget* parent)
      : Widget(obj_to_wrap, parent) {}
public:
  ~WrappedWidget() {}
};

}  // namespace LVGL_Wrapper
