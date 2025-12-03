#include "SettingsUi.hpp"
#include <ArduinoJson.h>
#include <iostream>
#include "api/cities.hpp"
#include "api/parameters/MeterologyCode.hpp"
#include "settings_storage.hpp"

void SettingsUI::load_values() {
  Settings s = SettingsStorage::load();
  if (s.city.size()) {
    for (size_t i = 0; i < city_list.size(); ++i) {
      if (city_list[i] == s.city) {
        city_dropdown->set_selected(i);
        break;
      }
    }
  } else {
    city_dropdown->set_selected(0);
  }

  for (size_t i = 0; i < parameter_list.size(); ++i) {
    if (parameter_list[i] == s.parameter) {
      parameter_dropdown->set_selected(i);
      break;
    }
  }
}

void SettingsUI::init() {
  set_size(LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_pad_all(m_obj, 20, 0);
  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(m_obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_gap(m_obj, 15, 0);
  set_scrollbar_mode(ScrollbarMode::Auto);

  title_label = Label::create(*this);
  title_label->set_text("Settings")
      .set_style_text_font(&lv_font_montserrat_28)
      .set_width(LV_PCT(100));
  title_label->set_style_text_align(LV_TEXT_ALIGN_CENTER);

  city_label = Label::create(*this);
  city_label->set_text("Select City:")
      .set_style_text_font(&lv_font_montserrat_18)
      .set_width(LV_PCT(100));

  std::string city_opts;
  for (auto i : kKnownCities) {
    city_opts += i.name;
    city_opts += "\n";
  }
  city_opts.pop_back();

  city_dropdown = Dropdown::create(*this);
  city_dropdown->set_options(city_opts.c_str()).set_width(LV_PCT(100));

  parameter_label = Label::create(*this);
  parameter_label->set_text("Graph Parameter:")
      .set_style_text_font(&lv_font_montserrat_18)
      .set_width(LV_PCT(100));

  parameter_list = {MeterologyCode::AirTemperature_DailyMax,
                    MeterologyCode::Relative_Humidity,
                    MeterologyCode::WindSpeed,
                    MeterologyCode::AirPressure_Reduced};
  std::string param_opts;
  for (auto i : parameter_list) {
    param_opts += i.toInfo().name;
    param_opts += "\n";
  }
  param_opts.pop_back();

  parameter_dropdown = Dropdown::create(*this);
  parameter_dropdown->set_options(param_opts.c_str()).set_width(LV_PCT(100));

  lv_obj_t* spacer = lv_obj_create(m_obj);
  lv_obj_set_size(spacer, 10, 20);
  lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(spacer, 0, 0);

  save_button = Button::create(*this);
  save_button->set_width(LV_PCT(100));
  save_button->set_height(50);
  save_button->set_style_bg_color(lv_color_hex(0x2196F3));

  auto save_label = Label::create(*save_button);
  save_label->set_text("SAVE SETTINGS")
      .set_style_text_font(&lv_font_montserrat_20)
      .center();

  load_values();

  save_button->on_clicked([this]() {
    Settings s;
    uint16_t sel = city_dropdown->get_selected();
    if (sel < city_list.size())
      s.city = city_list[sel];
    else
      s.city = city_list.empty() ? "" : city_list[0];

    uint16_t psel = parameter_dropdown->get_selected();
    if (psel < parameter_list.size())
      s.parameter = parameter_list[psel];
    else
      s.parameter = parameter_list.empty()
                        ? MeterologyCode(MeterologyCode::Unknown_Parameter)
                        : parameter_list[0];

    if (SettingsStorage::save(s)) {
      if (on_save_callback)
        on_save_callback(s);
    }
  });
}
