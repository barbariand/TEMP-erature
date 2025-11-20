#pragma once
#include "../core/Widget.hpp"
#include "./MenuCont.hpp"
#include "./MenuSection.hpp"

namespace LVGL_Wrapper {

class Menu;  // Forward declaration

/**
 * @brief Represents a "page" within a Menu.
 */
class MenuPage : public Widget {
  friend class Menu;

 protected:
  // Constructor for wrapping an existing LVGL object
  explicit MenuPage(BaseWidget* parent, lv_obj_t* obj_to_wrap)
      : Widget(obj_to_wrap, parent) {}

 public:
  /**
 * @brief Adds a new content container to this page.
 */
  std::shared_ptr<MenuCont> add_content() {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_cont = lv_menu_cont_create(m_obj);
    if (!c_cont)
      return nullptr;

    auto cont_ptr = std::shared_ptr<MenuCont>(new MenuCont(this, c_cont));
    this->register_child(cont_ptr);
    return cont_ptr;
  }

  /**
 * @brief Adds a new section (title/separator) to this page.
 */
  std::shared_ptr<MenuSection> add_section() {
    if (!m_obj)
      return nullptr;
    lv_obj_t* c_sect = lv_menu_section_create(m_obj);
    if (!c_sect)
      return nullptr;

    auto sect_ptr = std::shared_ptr<MenuSection>(new MenuSection(this, c_sect));
    this->register_child(sect_ptr);
    return sect_ptr;
  }
};

}  // namespace LVGL_Wrapper
// namespace LVGL_Wrapper
