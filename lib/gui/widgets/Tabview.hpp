#pragma once
#include "../core/Widget.hpp"
#include "./Tab.hpp"

namespace LVGL_Wrapper {

class Tabview : public Widget {
 private:
  explicit Tabview(BaseWidget* parent) : Widget(lv_tabview_create, parent) {}

 public:
  static std::shared_ptr<Tabview> create(BaseWidget& parent) {
    auto new_tabview = std::shared_ptr<Tabview>(new Tabview(&parent));
    parent.register_child(new_tabview);
    return new_tabview;
  }

  std::shared_ptr<Tab> add_tab(const char* name) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_page = lv_tabview_add_tab(m_obj, name);
    if (!c_page)
      return nullptr;

    auto page_ptr = std::shared_ptr<Tab>(new Tab(this, c_page));
    this->register_child(page_ptr);

    return page_ptr;
  }

  std::shared_ptr<Widget> get_content() {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_page = lv_tabview_get_content(m_obj);
    if (!c_page)
      return nullptr;

    auto* wrapper = static_cast<BaseWidget*>(lv_obj_get_user_data(c_page));
    if (wrapper) {
      return std::static_pointer_cast<Widget>(wrapper->shared_from_this());
    }

    auto page_ptr = std::shared_ptr<Tab>(new Tab(this, c_page));
    this->register_child(page_ptr);
    return page_ptr;
  }
};

}  // namespace LVGL_Wrapper
