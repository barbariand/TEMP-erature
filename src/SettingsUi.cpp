#include "SettingsUi.hpp"
#include "temp_gui/settings_storage.hpp"
#include <ArduinoJson.h>
#include <iostream>

void SettingsUI::load_values() {
  Settings prefs = SettingsStorage::load();
  // Sätt stad
  for (size_t i = 0; i < kKnownCities.size(); ++i) {
    if (kKnownCities[i].station == prefs.city) {
      place_dropdown->set_selected(i);
      break;
    }
  }
  // Sätt parameter
  for (size_t i = 0; i < metric_list.size(); ++i) {
    if (metric_list[i] == prefs.parameter) {
      metric_dropdown->set_selected(i);
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

  // Stad
  place_label = Label::create(*this);
  place_label->set_text("Select City:")
      .set_style_text_font(&lv_font_montserrat_18)
      .set_width(LV_PCT(100));

  std::string place_opts;
  for (auto& c : kKnownCities) {
    place_opts += c.name;
    place_opts += "\n";
  }
  place_opts.pop_back();

  place_dropdown = Dropdown::create(*this);
  place_dropdown->set_options(place_opts.c_str()).set_width(LV_PCT(100));

  // Parameter
  metric_label = Label::create(*this);
  metric_label->set_text("Graph Parameter:")
      .set_style_text_font(&lv_font_montserrat_18)
      .set_width(LV_PCT(100));

  metric_list = {MeterologyCode::AirTemperature_DailyMax,
                 MeterologyCode::Relative_Humidity,
                 MeterologyCode::WindSpeed,
                 MeterologyCode::AirPressure_Reduced};

  std::string metric_opts;
  for (auto i : metric_list) {
    metric_opts += i.toInfo().name;
    metric_opts += "\n";
  }
  metric_opts.pop_back();

  metric_dropdown = Dropdown::create(*this);
  metric_dropdown->set_options(metric_opts.c_str()).set_width(LV_PCT(100));

  // Save button
  save_button = Button::create(*this);
  save_button->set_width(LV_PCT(100));
  auto save_label = Label::create(*save_button);
  save_label->set_text("Save Default").center();

  // Ladda tidigare värden
  load_values();

  save_button->on_clicked([this]() {
    Settings prefs;
    uint16_t sel = place_dropdown->get_selected();
    if (sel < kKnownCities.size()) {
      prefs.city = kKnownCities[sel].station;
    }

    uint16_t psel = metric_dropdown->get_selected();
    if (psel < metric_list.size()) {
      prefs.parameter = metric_list[psel];
    }

    bool ok = SettingsStorage::save(prefs);
    if (!ok) {
      std::cout << "Failed to save settings" << std::endl;
    } else {
      if (on_save_callback) on_save_callback(prefs);
    }
  });
}
