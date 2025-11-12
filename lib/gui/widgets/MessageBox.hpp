#pragma once
#include "../core/Widget.hpp"
#include "./Button.hpp"
#include "./Label.hpp"
#include "./WrappedWidget.hpp"

namespace LVGL_Wrapper {
class MessageBox : public Widget {
 private:
  std::shared_ptr<Widget> m_header;
  std::shared_ptr<Widget> m_content;
  std::shared_ptr<Widget> m_footer;

  explicit MessageBox(BaseWidget* parent)
      : Widget(lv_msgbox_create, parent),
        m_header(nullptr),
        m_content(nullptr),
        m_footer(nullptr) {}

  explicit MessageBox(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent),
        m_header(nullptr),
        m_content(nullptr),
        m_footer(nullptr) {}

 public:
  static std::shared_ptr<MessageBox> create(BaseWidget& parent) {
    auto new_msgbox = std::shared_ptr<MessageBox>(new MessageBox(&parent));
    parent.register_child(new_msgbox);
    return new_msgbox;
  }

  std::shared_ptr<Label> add_title(const char* title) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_label = lv_msgbox_add_title(m_obj, title);
    if (!c_label)
      return nullptr;

    auto label_ptr = std::shared_ptr<Label>(new Label(this, c_label));
    this->register_child(label_ptr);
    return label_ptr;
  }

  std::shared_ptr<Button> add_header_button(const void* icon) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_btn = lv_msgbox_add_header_button(m_obj, icon);
    if (!c_btn)
      return nullptr;

    auto btn_ptr = std::shared_ptr<Button>(new Button(this, c_btn));
    this->register_child(btn_ptr);
    return btn_ptr;
  }

  std::shared_ptr<Button> add_close_button() {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_btn = lv_msgbox_add_close_button(m_obj);
    if (!c_btn)
      return nullptr;

    auto btn_ptr = std::shared_ptr<Button>(new Button(this, c_btn));
    this->register_child(btn_ptr);
    return btn_ptr;
  }

  std::shared_ptr<Label> add_text(const char* text) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_label = lv_msgbox_add_text(m_obj, text);
    if (!c_label)
      return nullptr;

    auto label_ptr = std::shared_ptr<Label>(new Label(this, c_label));
    this->register_child(label_ptr);
    return label_ptr;
  }

  std::shared_ptr<Button> add_footer_button(const char* text) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_btn = lv_msgbox_add_footer_button(m_obj, text);
    if (!c_btn)
      return nullptr;

    auto btn_ptr = std::shared_ptr<Button>(new Button(this, c_btn));
    this->register_child(btn_ptr);
    return btn_ptr;
  }

  std::shared_ptr<Widget> get_header() {
    if (!m_obj)
      return nullptr;
    if (m_header)
      return m_header;

    lv_obj_t* c_header = lv_msgbox_get_header(m_obj);
    if (!c_header)
      return nullptr;

    auto* wrapper = static_cast<BaseWidget*>(lv_obj_get_user_data(c_header));
    if (wrapper) {
      m_header = std::static_pointer_cast<Widget>(wrapper->shared_from_this());
    } else {
      m_header =
          std::shared_ptr<WrappedWidget>(new WrappedWidget(c_header, this));
      this->register_child(m_header);
    }
    return m_header;
  }

  std::shared_ptr<Widget> get_content() {
    if (!m_obj)
      return nullptr;
    if (m_content)
      return m_content;

    lv_obj_t* c_content = lv_msgbox_get_content(m_obj);
    if (!c_content)
      return nullptr;

    auto* wrapper = static_cast<BaseWidget*>(lv_obj_get_user_data(c_content));
    if (wrapper) {
      m_content = std::static_pointer_cast<Widget>(wrapper->shared_from_this());
    } else {
      m_content =
          std::shared_ptr<WrappedWidget>(new WrappedWidget(c_content, this));
      this->register_child(m_content);
    }
    return m_content;
  }

  std::shared_ptr<Widget> get_footer() {
    if (!m_obj)
      return nullptr;
    if (m_footer)
      return m_footer;

    lv_obj_t* c_footer = lv_msgbox_get_footer(m_obj);
    if (!c_footer)
      return nullptr;

    auto* wrapper = static_cast<BaseWidget*>(lv_obj_get_user_data(c_footer));
    if (wrapper) {
      m_footer = std::static_pointer_cast<Widget>(wrapper->shared_from_this());
    } else {
      m_footer =
          std::shared_ptr<WrappedWidget>(new WrappedWidget(c_footer, this));
      this->register_child(m_footer);
    }
    return m_footer;
  }

  std::shared_ptr<Label> get_title() {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_label = lv_msgbox_get_title(m_obj);
    if (!c_label)
      return nullptr;

    auto* wrapper = static_cast<BaseWidget*>(lv_obj_get_user_data(c_label));
    if (wrapper) {
      return std::static_pointer_cast<Label>(wrapper->shared_from_this());
    }

    auto label_ptr = std::shared_ptr<Label>(new Label(this, c_label));
    this->register_child(label_ptr);
    return label_ptr;
  }

  void close() {
    if (m_obj)
      lv_msgbox_close(m_obj);
  }

  void close_async() {
    if (m_obj)
      lv_msgbox_close_async(m_obj);
  }
};

}  // namespace LVGL_Wrapper
