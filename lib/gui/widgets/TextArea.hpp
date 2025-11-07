#pragma once
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class TextArea : public Widget, public TextProperties<TextArea> {
 private:
  explicit TextArea(BaseWidget* parent) : Widget(lv_textarea_create, parent) {}

 public:
  static std::shared_ptr<TextArea> create(BaseWidget& parent) {
    auto new_textarea = std::shared_ptr<TextArea>(new TextArea(&parent));
    parent.register_child(new_textarea);
    return new_textarea;
  }

  TextArea& add_text(const char* txt) {
    if (m_obj)
      lv_textarea_add_text(m_obj, txt);
    return *this;
  }

  TextArea& set_text(const char* txt) {
    if (m_obj)
      lv_textarea_set_text(m_obj, txt);
    return *this;
  }

  const char* get_text() const {
    return m_obj ? lv_textarea_get_text(m_obj) : "";
  }

  TextArea& set_placeholder_text(const char* txt) {
    if (m_obj)
      lv_textarea_set_placeholder_text(m_obj, txt);
    return *this;
  }

  TextArea& set_one_line(bool en) {
    if (m_obj)
      lv_textarea_set_one_line(m_obj, en);
    return *this;
  }
};

}  // namespace LVGL_Wrapper
