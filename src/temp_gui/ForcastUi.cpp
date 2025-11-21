#include "ForcastUi.hpp"
#include <memory>
#include "types/Enums.hpp"
void ForcastUI::update(ForecastSevenDay data) {
  auto report_list = data.get_all_mid_day_reports();

  size_t data_count = report_list.size();
  size_t current_ui_count = items.size();

  for (size_t i = 0; i < data_count; ++i) {

    if (i < current_ui_count) {
      items[i]->update_data(report_list[i]);
      items[i]->clear_flag(Flag::Hidden);
    } else {
      std::shared_ptr<ForcastUIItem> new_item =
          Component::create<ForcastUIItem>(*this);

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
void ForcastUI::init() {
  // Take up full space of the tile
  set_size(LV_PCT(100), LV_PCT(100));

  // Arrange items (ForcastUIItem) in a vertical column
  lv_obj_set_flex_flow(m_obj, LV_FLEX_FLOW_COLUMN);

  set_scrollbar_mode(ScrollbarMode::Auto);
  city_label = Label::create(*this);
  city_label->set_text("Karlskrona")
      .set_style_text_font(&lv_font_montserrat_28)
      .set_width(LV_PCT(100));

  city_label->set_style_text_align(LV_TEXT_ALIGN_CENTER);
}
