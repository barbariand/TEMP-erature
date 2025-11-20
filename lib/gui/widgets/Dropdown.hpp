#pragma once
#include "../core/Widget.hpp"
#include "./mixins/TextProperties.hpp"

namespace LVGL_Wrapper {

class Dropdown : public Widget, public TextProperties<Dropdown> {
  private:
    explicit Dropdown(BaseWidget* parent)
        : Widget(lv_dropdown_create, parent) {}

  public:
    static std::shared_ptr<Dropdown> create(BaseWidget& parent) {
        auto new_dropdown = std::shared_ptr<Dropdown>(new Dropdown(&parent));
        parent.register_child(new_dropdown);
        return new_dropdown;
    }

    Dropdown& set_options(const char* options) {
        if (m_obj)
            lv_dropdown_set_options(m_obj, options);
        return *this;
    }

    Dropdown& set_selected(uint16_t sel_opt) {
        if (m_obj)
            lv_dropdown_set_selected(m_obj, sel_opt);
        return *this;
    }

    uint16_t get_selected() const {
        return m_obj ? lv_dropdown_get_selected(m_obj) : 0;
    }

    Dropdown& set_dir(lv_dir_t dir) {
        if (m_obj)
            lv_dropdown_set_dir(m_obj, dir);
        return *this;
    }
};

} // namespace LVGL_Wrapper
