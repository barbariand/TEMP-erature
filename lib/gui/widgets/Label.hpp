#pragma once
#include <memory>
#include <string>
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class Label : public Widget, public TextProperties<Label> {
  friend class List;

 private:
  explicit Label(BaseWidget* parent) : Widget(lv_label_create, parent) {}

  explicit Label(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
  static std::shared_ptr<Label> create(BaseWidget& parent) {
    auto new_label = std::shared_ptr<Label>(new Label(&parent));
    parent.register_child(new_label);
    return new_label;
  }

  Label& set_text(const char* text) {
    if (m_obj)
      lv_label_set_text(m_obj, text);
    return *this;
  }
  Label& set_text(const std::string& text) { return set_text(text.c_str()); }
  Label& set_long_mode(lv_label_long_mode_t mode) {
    if (m_obj)
      lv_label_set_long_mode(m_obj, mode);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
