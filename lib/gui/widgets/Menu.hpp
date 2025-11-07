#pragma once
#include "../core/Widget.hpp"
#include "./MenuPage.hpp"

namespace LVGL_Wrapper {

class Menu : public Widget {
 private:
  explicit Menu(BaseWidget* parent) : Widget(lv_menu_create, parent) {}

 public:
  static std::shared_ptr<Menu> create(BaseWidget& parent) {
    auto new_menu = std::shared_ptr<Menu>(new Menu(&parent));
    parent.register_child(new_menu);
    return new_menu;
  }

  std::shared_ptr<MenuPage> add_page(const char* title) {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_page = lv_menu_page_create(m_obj, title);
    if (!c_page)
      return nullptr;

    auto page_ptr = std::shared_ptr<MenuPage>(new MenuPage(this, c_page));
    this->register_child(page_ptr);
    return page_ptr;
  }

  Menu& set_sidebar_page(MenuPage& page) {
    if (m_obj)
      lv_menu_set_sidebar_page(m_obj, page.raw());
    return *this;
  }

  Menu& set_main_page(MenuPage& page) {
    if (m_obj)
      lv_menu_set_page(m_obj, page.raw());
    return *this;
  }

  // Add other menu functions like set_mode_header, set_mode_root, etc.
};

}  // namespace LVGL_Wrapper
