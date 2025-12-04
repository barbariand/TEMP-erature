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

  // Example city list (adjust as needed)
  city_list = {"Karlskrona", "Stockholm", "Gothenburg", "Malmo", "Uppsala"};
  std::string city_opts;
  for (size_t i = 0; i < city_list.size(); ++i) {
    city_opts += city_list[i];
    if (i + 1 < city_list.size()) city_opts += "\n";
  }

  city_dropdown = Dropdown::create(*this);
  city_dropdown->set_options(city_opts.c_str()).set_width(LV_PCT(60));

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

  // Button row container (so Save Default and Reset are side-by-side)
  auto button_row = Component::create<Component>(*this);
  button_row->set_width(LV_PCT(100));
  // Use LVGL flex row on the raw object to place children horizontally
  lv_obj_set_flex_flow(button_row->raw(), LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_row(button_row->raw(), 8, 0);

  // Save Default button
  save_button = Button::create(*button_row);
  save_button->set_width(LV_PCT(46));
  auto save_label = Label::create(*save_button);
  save_label->set_text("Save Default").center();

  // Reset button (new)
  auto reset_button = Button::create(*button_row);
  reset_button->set_width(LV_PCT(46));
  auto reset_label = Label::create(*reset_button);
  reset_label->set_text("Reset").center();

  // Load current values from storage and apply to dropdowns
  load_values();

  // Lambda to apply changes immediately to UI (but DO NOT persist to SPIFFS)
  auto apply_changes = [this]() {
    Settings s;
    uint16_t sel = city_dropdown->get_selected();
    if (sel < city_list.size()) s.city = city_list[sel];
    else s.city = city_list.empty() ? "" : city_list[0];

    uint16_t psel = parameter_dropdown->get_selected();
    if (psel < parameter_list.size()) s.parameter = parameter_list[psel];
    else s.parameter = parameter_list.empty() ? "" : parameter_list[0];

    // Inform parent UI immediately (do NOT save here)
    if (on_save_callback) on_save_callback(s);
  };

  // Attach handlers: dropdown changes apply immediately but do not save to storage
  city_dropdown->on_value_changed([this, apply_changes]() {
    apply_changes();
  });

  parameter_dropdown->on_value_changed([this, apply_changes]() {
    apply_changes();
  });

  // Save Default button: persist current selections as startup defaults
  save_button->on_clicked([this]() {
    Settings s;
    uint16_t sel = city_dropdown->get_selected();
    if (sel < city_list.size()) s.city = city_list[sel];
    else s.city = city_list.empty() ? "" : city_list[0];

    uint16_t psel = parameter_dropdown->get_selected();
    if (psel < parameter_list.size()) s.parameter = parameter_list[psel];
    else s.parameter = parameter_list.empty() ? "" : parameter_list[0];

    bool ok = SettingsStorage::save(s);
    if (!ok) {
      std::cout << "Failed to save default settings" << std::endl;
    } else {
      // Inform parent (optional) that defaults changed
      if (on_save_callback) on_save_callback(s);
    }
  });

  // Reset button: restore dropdown selections from stored defaults (do not change storage)
  reset_button->on_clicked([this]() {
    // Reload values from storage into the dropdowns
    load_values();
    // Notify parent about the restored (default) values
    Settings s = SettingsStorage::load();
    if (on_save_callback) on_save_callback(s);
  });
}
