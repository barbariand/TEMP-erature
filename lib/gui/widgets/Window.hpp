#pragma once
#include "Button.hpp"
#include "Label.hpp"
#include "core/Widget.hpp"
#include "widgets/WrappedWidget.hpp"

namespace LVGL_Wrapper {

class Window : public Widget {
 private:
  std::shared_ptr<Widget> m_content_page;

  explicit Window(BaseWidget* parent) : Widget(lv_win_create, parent) {}

 public:
  static std::shared_ptr<Window> create(BaseWidget& parent) {
    auto new_window = std::shared_ptr<Window>(new Window(&parent));
    parent.register_child(new_window);
    return new_window;
  }

  std::shared_ptr<Label> add_title(const char* txt) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_label = lv_win_add_title(m_obj, txt);
    if (!c_label)
      return nullptr;

    auto label_ptr = std::shared_ptr<Label>(new Label(this, c_label));
    this->register_child(label_ptr);
    return label_ptr;
  }

  std::shared_ptr<Button> add_header_btn(const void* icon, int32_t w) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_btn = lv_win_add_button(m_obj, icon, w);
    if (!c_btn)
      return nullptr;

    auto btn_ptr = std::shared_ptr<Button>(new Button(this, c_btn));
    this->register_child(btn_ptr);
    return btn_ptr;
  }

  std::shared_ptr<Widget> get_content() {
    if (m_content_page)
      return m_content_page;
    if (!m_obj)
      return nullptr;

    lv_obj_t* c_content = lv_win_get_content(m_obj);
    if (!c_content)
      return nullptr;

    m_content_page =
        std::shared_ptr<WrappedWidget>(new WrappedWidget(c_content, this));
    this->register_child(m_content_page);
    return m_content_page;
  }
};

}  // namespace LVGL_Wrapper
