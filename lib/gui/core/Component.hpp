#pragma once
#include "./Widget.hpp"

namespace LVGL_Wrapper {

class Component : public Widget {
 protected:
  explicit Component(BaseWidget* parent) : Widget(lv_obj_create, parent) {}

 public:
  virtual ~Component() = default;
  virtual void init() {}

  template <typename T>
  static std::shared_ptr<T> create(BaseWidget& parent) {
    auto new_component = std::shared_ptr<T>(new T(&parent));
    parent.register_child(new_component);

    new_component->init();

    return new_component;
  }

 protected:
  template <typename WidgetType>
  std::shared_ptr<WidgetType> make_child() {
    return WidgetType::create(*this);
  }
};
}  // namespace LVGL_Wrapper
