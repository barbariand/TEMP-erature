#include "SettingsUi.hpp"
#include "settings_storage.hpp"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <iostream>

void SettingsUI::load_values() {
  Settings s = SettingsStorage::load();
  // Set city dropdown selection based on stored city
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

  // Set parameter selection
  for (size_t i = 0; i < parameter_list.size(); ++i) {
    if (parameter_list[i] == s.parameter) {
      parameter_dropdown->set_selected(i);
      break;
    }
  }
}

void SettingsUI::init() {
  set_size(LV_PCT(100), LV_PCT(100));

  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);
  set_scrollbar_mode(ScrollbarMode::Auto);

  title_label = Label::create(*this);
  title_label->set_text("Settings")
      .set_style_text_font(&lv_font_montserrat_28)
      .set_width(LV_PCT(100));
  title_label->set_style_text_align(LV_TEXT_ALIGN_CENTER);

  // City (dropdown)
  city_label = Label::create(*this);
  city_label->set_text("City:")
      .set_style_text_font(&lv_font_montserrat_22)
      .set_width(LV_PCT(100));

  // Define cities (example list)
  city_list = {"Karlskrona", "Stockholm", "Gothenburg", "Malmo", "Uppsala"};
  std::string city_opts;
  for (size_t i = 0; i < city_list.size(); ++i) {
    city_opts += city_list[i];
    if (i + 1 < city_list.size()) city_opts += "\n";
  }

  city_dropdown = Dropdown::create(*this);
  city_dropdown->set_options(city_opts.c_str()).set_width(LV_PCT(60));

  // Units
  // Parameter selection
  parameter_label = Label::create(*this);
  parameter_label->set_text("Parameter:")
      .set_style_text_font(&lv_font_montserrat_22)
      .set_width(LV_PCT(100));

  parameter_list = {"Temperature", "Humidity", "Wind Speed"};
  std::string param_opts;
  for (size_t i = 0; i < parameter_list.size(); ++i) {
    param_opts += parameter_list[i];
    if (i + 1 < parameter_list.size()) param_opts += "\n";
  }

  parameter_dropdown = Dropdown::create(*this);
  parameter_dropdown->set_options(param_opts.c_str()).set_width(LV_PCT(60));

  // Save button
  save_button = Button::create(*this);
  save_button->set_width(LV_PCT(60));
  auto save_label = Label::create(*save_button);
  save_label->set_text("Save").center();

  // Load existing values
  load_values();

  save_button->on_clicked([this]() {
    Settings s;
    // Read selected city
    uint16_t sel = city_dropdown->get_selected();
    if (sel < city_list.size()) s.city = city_list[sel];
    else s.city = city_list.empty() ? "" : city_list[0];

    // Read selected parameter
    uint16_t psel = parameter_dropdown->get_selected();
    if (psel < parameter_list.size()) s.parameter = parameter_list[psel];
    else s.parameter = parameter_list.empty() ? "" : parameter_list[0];

    bool ok = SettingsStorage::save(s);
    if (!ok) {
      std::cout << "Failed to save settings" << std::endl;
    } else {
      if (on_save_callback) on_save_callback(s);
    }
  });
}
