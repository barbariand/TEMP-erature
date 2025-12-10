#include "ForcastUi.hpp"
#include <memory>
#include "types/Enums.hpp"
#include "temp_gui/Temp_gui.hpp"

using namespace LVGL_Wrapper;

void ForcastUI::init() {
  set_size(LV_PCT(100), LV_PCT(100));
  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);

  lv_obj_set_style_pad_hor(m_obj, 10, 0);
  lv_obj_set_style_pad_top(m_obj, 10, 0);
  lv_obj_set_style_pad_gap(m_obj, 10, 0);
  set_scrollbar_mode(ScrollbarMode::Auto);

  items.reserve(7);

  city_label = Label::create(*this);
  city_label->set_text("Loading...")
      .set_style_text_font(&lv_font_montserrat_28)
      .set_width(LV_PCT(100));

  city_label->set_style_text_align(LV_TEXT_ALIGN_CENTER);

  lv_obj_t* btn = lv_btn_create(m_obj);
  lv_obj_set_size(btn, 36, 36);
  lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -8, 8);
  lv_obj_move_foreground(btn);

  lv_obj_t* lbl = lv_label_create(btn);
  lv_label_set_text(lbl, LV_SYMBOL_SETTINGS);
  lv_obj_center(lbl);

  lv_obj_add_event_cb(btn, [](lv_event_t*){
    if (g_temp_gui_instance) g_temp_gui_instance->go_to_settings_tile();
  }, LV_EVENT_CLICKED, nullptr);
}

void ForcastUI::update(const ForecastSevenDay& data) {

  ForecastSevenDay& non_const_data = const_cast<ForecastSevenDay&>(data);
  const auto& report_list = non_const_data.get_all_mid_day_reports();

  size_t data_count = report_list.size();
  size_t current_ui_count = items.size();

  for (size_t i = 0; i < data_count; ++i) {
    if (i < current_ui_count) {
      items[i]->update_data(report_list[i]);
      items[i]->clear_flag(Flag::Hidden);
    } else {
      auto new_item = Component::create<ForcastUIItem>(*this);
      new_item->update_data(report_list[i]);
      items.push_back(new_item);
    }
  }

  if (current_ui_count > data_count) {
    for (size_t i = data_count; i < current_ui_count; ++i) {
      items[i]->add_flag(Flag::Hidden);
    }
  }
}

void ForcastUI::set_city(const char* city) {
  if (!city_label)
    return;
  city_label->set_text(city);
}
