#pragma once
#include "./Widget.hpp"

namespace LVGL_Wrapper {

class Component : public Widget {
 protected:
  explicit Component(BaseWidget* parent) : Widget(lv_obj_create, parent) {}

 public:
  virtual ~Component() {}

  static std::shared_ptr<Component> create(BaseWidget& parent) {
    auto new_component = std::shared_ptr<Component>(new Component(&parent));
    parent.register_child(new_component);
    return new_component;
  }
};

}  // namespace LVGL_Wrapper
