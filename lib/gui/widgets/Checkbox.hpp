#pragma once
#include "../core/Widget.hpp"
#include "../types/Enums.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class Checkbox : public Widget, public TextProperties<Checkbox> {
 private:
  explicit Checkbox(BaseWidget* parent) : Widget(lv_checkbox_create, parent) {}

 public:
  static std::shared_ptr<Checkbox> create(BaseWidget& parent) {
    auto new_checkbox = std::shared_ptr<Checkbox>(new Checkbox(&parent));
    parent.register_child(new_checkbox);
    return new_checkbox;
  }

  Checkbox& set_text(const char* text) {
    if (m_obj)
      lv_checkbox_set_text(m_obj, text);
    return *this;
  }

  const char* get_text() const {
    return m_obj ? lv_checkbox_get_text(m_obj) : "";
  }

  bool is_checked() const {
    return m_obj ? lv_obj_has_state(m_obj,
                                    static_cast<lv_state_t>(State::Checked))
                 : false;
  }
};

}  // namespace LVGL_Wrapper
