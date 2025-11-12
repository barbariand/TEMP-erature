#pragma once
#include "../core/Widget.hpp"
#include "./TextArea.hpp"

namespace LVGL_Wrapper {

class Keyboard : public Widget {
 private:
  explicit Keyboard(BaseWidget* parent) : Widget(lv_keyboard_create, parent) {}

 public:
  static std::shared_ptr<Keyboard> create(BaseWidget& parent) {
    auto new_keyboard = std::shared_ptr<Keyboard>(new Keyboard(&parent));
    parent.register_child(new_keyboard);
    return new_keyboard;
  }

  Keyboard& set_textarea(TextArea& ta) {
    if (m_obj)
      lv_keyboard_set_textarea(m_obj, ta.raw());
    return *this;
  }

  Keyboard& set_mode(lv_keyboard_mode_t mode) {
    if (m_obj)
      lv_keyboard_set_mode(m_obj, mode);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
