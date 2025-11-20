#pragma once
#include "../core/Widget.hpp"
#include <string>
#include <map>

namespace LVGL_Wrapper {

class ImageButton : public Widget {
  private:
    explicit ImageButton(BaseWidget* parent)
        : Widget(lv_imagebutton_create, parent) {}

    std::map<lv_imagebutton_state_t, std::string> m_src_paths;
    std::map<lv_imagebutton_state_t, const void*> m_src_vars;

    void set_src_internal(lv_imagebutton_state_t state, const void* src) {
        m_src_vars[state] = src;
        m_src_paths.erase(state);
        if (m_obj)
            lv_imagebutton_set_src(m_obj, state, nullptr, src, nullptr);
    }

    void set_src_internal(lv_imagebutton_state_t state, const char* path) {
        m_src_paths[state] = path;
        m_src_vars.erase(state);
        if (m_obj)
            lv_imagebutton_set_src(m_obj, state, nullptr,
                                   m_src_paths[state].c_str(), nullptr);
    }

  public:
    static std::shared_ptr<ImageButton> create(BaseWidget& parent) {
        auto new_imgbtn = std::shared_ptr<ImageButton>(new ImageButton(&parent));
        parent.register_child(new_imgbtn);
        return new_imgbtn;
    }

    ImageButton& set_src_released(const char* path) {
        set_src_internal(LV_IMAGEBUTTON_STATE_RELEASED, path);
        return *this;
    }
    ImageButton& set_src_released(const void* var) {
        set_src_internal(LV_IMAGEBUTTON_STATE_RELEASED, var);
        return *this;
    }

    ImageButton& set_src_pressed(const char* path) {
        set_src_internal(LV_IMAGEBUTTON_STATE_PRESSED, path);
        return *this;
    }
    ImageButton& set_src_pressed(const void* var) {
        set_src_internal(LV_IMAGEBUTTON_STATE_PRESSED, var);
        return *this;
    }

    ImageButton& set_src_checked_released(const char* path) {
        set_src_internal(LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, path);
        return *this;
    }
    ImageButton& set_src_checked_released(const void* var) {
        set_src_internal(LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, var);
        return *this;
    }

    ImageButton& set_src_checked_pressed(const char* path) {
        set_src_internal(LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, path);
        return *this;
    }
    ImageButton& set_src_checked_pressed(const void* var) {
        set_src_internal(LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, var);
        return *this;
    }

};

} // namespace LVGL_Wrapper
