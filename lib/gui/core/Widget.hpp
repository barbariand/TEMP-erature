#pragma once

#include "../types/Color.hpp"
#include "../types/Enums.hpp"
#include "../types/StyleSelector.hpp"
#include "./BaseWidget.hpp"

namespace LVGL_Wrapper {

class Widget : public BaseWidget {
  friend class WrappedWidget;

 protected:
  Widget(LvCreateFunc create_func, BaseWidget* parent)
      : BaseWidget(create_func, parent) {}

  explicit Widget(lv_obj_t* obj_to_wrap) : BaseWidget(obj_to_wrap) {}

  Widget(lv_obj_t* obj_to_wrap, BaseWidget* parent)
      : BaseWidget(obj_to_wrap, parent) {}

 public:
  virtual ~Widget() {}

  Widget& add_flag(Flag::Type f) {
    if (m_obj)
      lv_obj_add_flag(m_obj, (lv_obj_flag_t)f);
    return *this;
  }

  Widget& clear_flag(Flag::Type f) {
    if (m_obj)
      lv_obj_clear_flag(m_obj, (lv_obj_flag_t)f);
    return *this;
  }

  Widget& add_state(State::Type s) {
    if (m_obj)
      lv_obj_add_state(m_obj, (lv_state_t)s);
    return *this;
  }

  Widget& clear_state(State::Type s) {
    if (m_obj)
      lv_obj_clear_state(m_obj, (lv_state_t)s);
    return *this;
  }

  bool has_state(State::Type s) const {
    return m_obj ? lv_obj_has_state(m_obj, (lv_state_t)s) : false;
  }

  Widget& set_pos(int32_t x, int32_t y) {
    if (m_obj)
      lv_obj_set_pos(m_obj, x, y);
    return *this;
  }

  Widget& set_x(int32_t x) {
    if (m_obj)
      lv_obj_set_x(m_obj, x);
    return *this;
  }

  Widget& set_y(int32_t y) {
    if (m_obj)
      lv_obj_set_y(m_obj, y);
    return *this;
  }

  Widget& align(Align::Type align_type, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    if (m_obj)
      lv_obj_align(m_obj, (lv_align_t)align_type, x_ofs, y_ofs);
    return *this;
  }
  Widget& align_to(const BaseWidget& base, Align::Type align_type,
                   int32_t x_ofs = 0, int32_t y_ofs = 0) {
    if (m_obj)
      lv_obj_align_to(m_obj, base.raw(), static_cast<lv_align_t>(align_type),
                      x_ofs, y_ofs);
    return *this;
  }

  Widget& center() {
    if (m_obj)
      lv_obj_center(m_obj);
    return *this;
  }

  Widget& set_size(int32_t w, int32_t h) {
    if (m_obj)
      lv_obj_set_size(m_obj, w, h);
    return *this;
  }

  Widget& set_width(int32_t w) {
    if (m_obj)
      lv_obj_set_width(m_obj, w);
    return *this;
  }

  Widget& set_height(int32_t h) {
    if (m_obj)
      lv_obj_set_height(m_obj, h);
    return *this;
  }

  Widget& set_width_content() {
    if (m_obj)
      lv_obj_set_width(m_obj, LV_SIZE_CONTENT);
    return *this;
  }

  Widget& set_height_content() {
    if (m_obj)
      lv_obj_set_height(m_obj, LV_SIZE_CONTENT);
    return *this;
  }

  int32_t get_width() const { return m_obj ? lv_obj_get_width(m_obj) : 0; }

  int32_t get_height() const { return m_obj ? lv_obj_get_height(m_obj) : 0; }

  Widget& set_layout(uint32_t layout) {
    if (m_obj)
      lv_obj_set_layout(m_obj, layout);
    return *this;
  }

  Widget& set_style_bg_opa(Opa::Type opa, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_bg_opa(m_obj, (lv_opa_t)opa, selector);
    return *this;
  }

  Widget& set_style_bg_color(Color color, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_bg_color(m_obj, color, selector);
    return *this;
  }
  Widget& set_style_radius(int32_t radius, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_radius(m_obj, radius, selector);
    return *this;
  }

  Widget& set_style_pad_all(int32_t pad, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_pad_all(m_obj, pad, selector);
    return *this;
  }

  Widget& set_style_pad_row(int32_t pad, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_pad_row(m_obj, pad, selector);
    return *this;
  }

  Widget& set_style_pad_column(int32_t pad, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_pad_column(m_obj, pad, selector);
    return *this;
  }

  Widget& set_style_border_width(int32_t width, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_border_width(m_obj, width, selector);
    return *this;
  }

  Widget& set_style_border_color(const Color& color,
                                 StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_border_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_style_border_opa(Opa::Type opa, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_border_opa(m_obj, (lv_opa_t)opa, selector);
    return *this;
  }

  Widget& set_style_outline_width(int32_t width, StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_outline_width(m_obj, width, selector);
    return *this;
  }

  Widget& set_style_outline_color(const Color& color,
                                  StyleSelector selector = {}) {
    if (m_obj)
      lv_obj_set_style_outline_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_scrollbar_mode(ScrollbarMode::Type mode) {
    if (m_obj)
      lv_obj_set_scrollbar_mode(m_obj, (lv_scrollbar_mode_t)mode);
    return *this;
  }

  Widget& set_scroll_dir(Direction::Type dir) {
    if (m_obj)
      lv_obj_set_scroll_dir(m_obj, (lv_dir_t)dir);
    return *this;
  }

  Widget& on_event(Event::Type event_code,
                   std::function<void(lv_event_t*)> callback) {
    if (m_obj) {
      m_callbacks[(lv_event_code_t)event_code] = std::move(callback);
      lv_obj_add_event_cb(m_obj, universal_event_handler,
                          (lv_event_code_t)event_code, this);
    }
    return *this;
  }

  Widget& on_event(Event::Type event_code, std::function<void()> callback) {
    return on_event(event_code,
                    [cb = std::move(callback)](lv_event_t* e) { cb(); });
  }

  Widget& on_clicked(std::function<void()> callback) {
    return on_event(Event::Clicked, std::move(callback));
  }

  Widget& on_value_changed(std::function<void()> callback) {
    return on_event(Event::ValueChanged, std::move(callback));
  }

  Widget& on_pressed(std::function<void()> callback) {
    return on_event(Event::Pressed, std::move(callback));
  }

  Widget& on_released(std::function<void()> callback) {
    return on_event(Event::Released, std::move(callback));
  }
};

}  // namespace LVGL_Wrapper
