#pragma once
#include "../assets/font.hpp"
#include "GUI.hpp"
#include "api/data/TimeSeriesItem.hpp"
#include "types/StyleSelector.hpp"
using namespace LVGL_Wrapper;
class ForcastUIItem : public Component {
  std::shared_ptr<Label> icon;
  std::shared_ptr<Label> date;
  std::shared_ptr<Label> temperature;

 public:
  void init() override {

    set_width(LV_PCT(100));
    set_height(LV_SIZE_CONTENT);

    lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_ROW);

    lv_obj_set_flex_align(m_obj, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    set_style_pad_all(10);

    icon = Label::create(*this);
    icon->set_style_text_font(&weather_font_40, StyleSelector{});
    date = Label::create(*this);

    temperature = Label::create(*this);
  }
  explicit ForcastUIItem(BaseWidget* parent) : Component(parent) {}
  void update_data(const TimeSeriesItem& item);
};
