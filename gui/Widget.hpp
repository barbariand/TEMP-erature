#pragma once

extern "C" {
#include "lvgl.h"
}

namespace LVGL_Wrapper {

class BaseWidget {
 protected:
  lv_obj_t* m_obj;

  /**
   * @brief Default constructor. Initializes as a null object.
   */
  BaseWidget() : m_obj(nullptr) {}

  /**
   * @brief Protected constructor to wrap an existing object.
   */
  explicit BaseWidget(lv_obj_t* obj) : m_obj(obj) {
    wrap(obj);  // Use wrap to set user data
  }

  /**
   * @brief Common wrapping logic, usable by create() or sub-classes.
   */
  void wrap(lv_obj_t* obj) {
    m_obj = obj;
    if (m_obj) {
      // Set user data to point to this C++ instance
      lv_obj_set_user_data(m_obj, this);
    }
  }

 public:
  virtual ~BaseWidget() {}

  lv_obj_t* raw() const { return m_obj; }
};

class Widget : public BaseWidget {
 protected:
  /**
   * @brief Default constructor for two-stage init.
   */
  Widget() : BaseWidget(nullptr) {}

  /**
   * @brief Protected constructor to wrap an existing object.
   */
  explicit Widget(lv_obj_t* obj) : BaseWidget(obj) {}

 public:
  virtual ~Widget() {}

  // --- Position & Alignment ---

  Widget& set_pos(lv_coord_t x, lv_coord_t y) {
    if (m_obj)
      lv_obj_set_pos(m_obj, x, y);
    return *this;
  }

  Widget& set_x(lv_coord_t x) {
    if (m_obj)
      lv_obj_set_x(m_obj, x);
    return *this;
  }

  Widget& set_y(lv_coord_t y) {
    if (m_obj)
      lv_obj_set_y(m_obj, y);
    return *this;
  }

  Widget& align(lv_align_t align_type, lv_coord_t x_ofs = 0,
                lv_coord_t y_ofs = 0) {
    if (m_obj)
      lv_obj_align(m_obj, align_type, x_ofs, y_ofs);
    return *this;
  }

  Widget& align_to(const BaseWidget& base, lv_align_t align_type,
                   lv_coord_t x_ofs = 0, lv_coord_t y_ofs = 0) {
    if (m_obj)
      lv_obj_align_to(m_obj, base.raw(), align_type, x_ofs, y_ofs);
    return *this;
  }

  Widget& center() {
    if (m_obj)
      lv_obj_center(m_obj);
    return *this;
  }

  // --- Size ---

  Widget& set_size(lv_coord_t w, lv_coord_t h) {
    if (m_obj)
      lv_obj_set_size(m_obj, w, h);
    return *this;
  }

  Widget& set_width(lv_coord_t w) {
    if (m_obj)
      lv_obj_set_width(m_obj, w);
    return *this;
  }

  Widget& set_height(lv_coord_t h) {
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

  // --- Flags & State ---

  Widget& add_flag(lv_obj_flag_t f) {
    if (m_obj)
      lv_obj_add_flag(m_obj, f);
    return *this;
  }

  Widget& clear_flag(lv_obj_flag_t f) {
    if (m_obj)
      lv_obj_clear_flag(m_obj, f);
    return *this;
  }

  Widget& add_state(lv_state_t s) {
    if (m_obj)
      lv_obj_add_state(m_obj, s);
    return *this;
  }

  Widget& clear_state(lv_state_t s) {
    if (m_obj)
      lv_obj_clear_state(m_obj, s);
    return *this;
  }

  // --- Hierarchy ---

  Widget& set_parent(BaseWidget& parent) {
    if (m_obj)
      lv_obj_set_parent(m_obj, parent.raw());
    return *this;
  }

  // --- Scrolling ---

  Widget& set_scrollbar_mode(lv_scrollbar_mode_t mode) {
    if (m_obj)
      lv_obj_set_scrollbar_mode(m_obj, mode);
    return *this;
  }

  Widget& set_scroll_dir(lv_dir_t dir) {
    if (m_obj)
      lv_obj_set_scroll_dir(m_obj, dir);
    return *this;
  }

  // --- Events ---

  Widget& add_event_cb(lv_event_cb_t event_cb, lv_event_code_t filter,
                       void* user_data) {
    if (m_obj)
      lv_obj_add_event_cb(m_obj, event_cb, filter, user_data);
    return *this;
  }

  // --- Local Styles (Common) ---

  Widget& set_style_bg_color(lv_color_t color,
                             lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_bg_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_style_bg_opa(lv_opa_t opa,
                           lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_bg_opa(m_obj, opa, selector);
    return *this;
  }

  Widget& set_style_text_color(lv_color_t color,
                               lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_text_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_style_text_font(const lv_font_t* font,
                              lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_text_font(m_obj, font, selector);
    return *this;
  }

  Widget& set_style_radius(lv_coord_t radius,
                           lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_radius(m_obj, radius, selector);
    return *this;
  }

  Widget& set_style_border_width(lv_coord_t width,
                                 lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_border_width(m_obj, width, selector);
    return *this;
  }

  Widget& set_style_border_color(lv_color_t color,
                                 lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_border_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_style_outline_width(lv_coord_t width,
                                  lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_outline_width(m_obj, width, selector);
    return *this;
  }

  Widget& set_style_outline_color(lv_color_t color,
                                  lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_outline_color(m_obj, color, selector);
    return *this;
  }

  Widget& set_style_pad_all(lv_coord_t pad,
                            lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_pad_all(m_obj, pad, selector);
    return *this;
  }
  Widget& set_style_pad_hor(lv_coord_t pad,
                            lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_pad_hor(m_obj, pad, selector);
    return *this;
  }
  Widget& set_style_pad_ver(lv_coord_t pad,
                            lv_style_selector_t selector = LV_PART_MAIN) {
    if (m_obj)
      lv_obj_set_style_pad_ver(m_obj, pad, selector);
    return *this;
  }

  // --- Layout (Flex & Grid) ---

  Widget& set_layout(uint32_t layout) {
    if (m_obj)
      lv_obj_set_layout(m_obj, layout);
    return *this;
  }

  Widget& set_flex_flow(lv_flex_flow_t flow) {
    if (m_obj)
      lv_obj_set_flex_flow(m_obj, flow);
    return *this;
  }

  Widget& set_flex_grow(uint8_t grow) {
    if (m_obj)
      lv_obj_set_flex_grow(m_obj, grow);
    return *this;
  }

  Widget& set_grid_cell(lv_grid_align_t col_align, uint8_t col_pos,
                        uint8_t col_span, lv_grid_align_t row_align,
                        uint8_t row_pos, uint8_t row_span) {
    if (m_obj)
      lv_obj_set_grid_cell(m_obj, col_align, col_pos, col_span, row_align,
                           row_pos, row_span);
    return *this;
  }
};

}  // namespace LVGL_Wrapper

