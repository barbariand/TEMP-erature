#pragma once
#include "../core/Widget.hpp"
#include <string>

namespace LVGL_Wrapper {

class Image : public Widget {
  protected:
    Image(LvCreateFunc create_func, BaseWidget* parent)
        : Widget(create_func, parent) {}

  private:
    explicit Image(BaseWidget* parent) : Widget(lv_image_create, parent) {}

    std::string m_src_path;

  public:
    static std::shared_ptr<Image> create(BaseWidget& parent) {
        auto new_image = std::shared_ptr<Image>(new Image(&parent));
        parent.register_child(new_image);
        return new_image;
    }

    Image& set_src(const char* path) {
        if (m_obj) {
            m_src_path = path;
            lv_image_set_src(m_obj, m_src_path.c_str());
        }
        return *this;
    }

    Image& set_src(const void* src_var) {
        if (m_obj) {
            m_src_path.clear();
            lv_image_set_src(m_obj, src_var);
        }
        return *this;
    }

    Image& set_rotation(int16_t angle_deci_deg) {
        if (m_obj)
            lv_image_set_rotation(m_obj, angle_deci_deg);
        return *this;
    }

    Image& set_zoom(uint16_t zoom_factor) {
        if (m_obj)
            lv_image_set_scale(m_obj, zoom_factor);
        return *this;
    }
};

} // namespace LVGL_Wrapper
