#pragma once
#include "../assets/font.hpp"
#include "GUI.hpp"
#include "api/data/TimeSeriesItem.hpp"
#include "font/lv_font.h"
#include "types/StyleSelector.hpp"

using namespace LVGL_Wrapper;

class ForcastUIItem : public Component {
  std::shared_ptr<Label> icon;
  std::shared_ptr<Label> date;
  std::shared_ptr<Label> temperature;

 public:
  explicit ForcastUIItem(BaseWidget* parent) : Component(parent) {}

  void init() override {
    set_width(LV_PCT(100));
    set_height(55); 

    // Layout
    lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(m_obj, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(m_obj, 10, 0); 
    
    // Border bottom
    lv_obj_set_style_bg_opa(m_obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(m_obj, 0, 0);
    lv_obj_set_style_border_width(m_obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT); 
    lv_obj_set_style_border_side(m_obj, LV_BORDER_SIDE_BOTTOM, 0); 
    lv_obj_set_style_border_color(m_obj, lv_color_hex(0xE0E0E0), 0); 

    // Datum
    date = Label::create(*this);
    date->set_style_text_font(&lv_font_montserrat_18);
    date->set_style_text_color(Color::from_lv_color(lv_color_hex(0x333333)));

    // Ikon
    icon = Label::create(*this);
    icon->set_style_text_font(&weather_font_40, StyleSelector{}); 
    icon->set_style_text_color(Color::from_lv_color(lv_color_hex(0x555555)));

    // Temp
    temperature = Label::create(*this);
    temperature->set_style_text_font(&lv_font_montserrat_20);
    temperature->set_style_text_color(Color::Black);
  }

  void update_data(const TimeSeriesItem& item);
};