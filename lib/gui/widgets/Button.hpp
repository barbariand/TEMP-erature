#pragma once
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class Button : public Widget, public TextProperties<Button> {


 private:
  explicit Button(BaseWidget* parent) : Widget(lv_button_create, parent) {}
  explicit Button(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
  static std::shared_ptr<Button> create(BaseWidget& parent) {
    auto new_button = std::shared_ptr<Button>(new Button(&parent));
    parent.register_child(new_button);
    return new_button;
  }
};

}  // namespace LVGL_Wrapper
