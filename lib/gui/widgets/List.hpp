#pragma once
#include "../core/Widget.hpp"
#include "./Button.hpp"
#include "./Label.hpp"

namespace LVGL_Wrapper {

class List : public Widget {
 private:
  explicit List(BaseWidget* parent) : Widget(lv_list_create, parent) {}

 public:
  static std::shared_ptr<List> create(BaseWidget& parent) {
    auto new_list = std::shared_ptr<List>(new List(&parent));
    parent.register_child(new_list);
    return new_list;
  }

  std::shared_ptr<Label> add_text(const char* txt) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_label = lv_list_add_text(m_obj, txt);
    if (!c_label)
      return nullptr;

    auto label_ptr = std::shared_ptr<Label>(new Label(this, c_label));
    this->register_child(label_ptr);
    return label_ptr;
  }

  std::shared_ptr<Button> add_button(const char* icon, const char* txt) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_btn = lv_list_add_btn(m_obj, icon, txt);
    if (!c_btn)
      return nullptr;

    auto btn_ptr = std::shared_ptr<Button>(new Button(this, c_btn));
    this->register_child(btn_ptr);
    return btn_ptr;
  }
};

}  // namespace LVGL_Wrapper
