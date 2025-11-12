#pragma once
#include "../core/Widget.hpp"
#include "./Image.hpp"
#include "./Label.hpp"

namespace LVGL_Wrapper {

class MenuPage;  // Forward declaration

class MenuCont : public Widget {
  friend class MenuPage;

 protected:
  explicit MenuCont(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
  std::shared_ptr<Label> add_item(const char* text) {
    auto label = Label::create(*this);
    label->set_text(text);
    label->set_long_mode(LV_LABEL_LONG_SCROLL_CIRCULAR);
    label->set_width(LV_PCT(100));
    return label;
  }
};

}  // namespace LVGL_Wrapper
