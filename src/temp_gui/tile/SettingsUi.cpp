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
  save_label->set_text("Save").center();
  save_label->set_text("SAVE SETTINGS")
      .set_style_text_font(&lv_font_montserrat_20)
      .center();

  save_label->set_text("Save Default").center();

  reset_button = Button::create(*button_row);
  reset_button->set_width(LV_PCT(46));
  auto reset_label = Label::create(*reset_button);
  reset_label->set_text("Reset").center();

  // Load current values from storage and apply to dropdowns
  load_values();

  auto apply_changes = [this]() {
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

    // Inform parent UI immediately (do NOT save here)
    if (on_save_callback)
      on_save_callback(s);
  };

  // Attach handlers: dropdown changes apply immediately but do not save to storage
  city_dropdown->on_value_changed([this, apply_changes]() { apply_changes(); });

  parameter_dropdown->on_value_changed(
      [this, apply_changes]() { apply_changes(); });

  // Save Default button: persist current selections as startup defaults
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

    if (bool ok = SettingsStorage::save(s)) {

      if (!ok) {
        std::cout << "Failed to save settings" << std::endl;
        std::cout << "Failed to save default settings" << std::endl;
      }

      if (on_save_callback)
        on_save_callback(s);
    }
    reset_button->on_clicked([this]() {
      load_values();
      Settings s = SettingsStorage::load();
      if (on_save_callback)
        on_save_callback(s);
    });
  });
};
