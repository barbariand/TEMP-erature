#include "ForcastUiItem.hpp"

void ForcastUIItem::update_data(const TimeSeriesItem& item) {
  // Datumformat: "Mon 30"
  std::string d = item.time.toString("%a %d"); 
  date->set_text(d.c_str());

  char temp_buf[16];
  snprintf(temp_buf, sizeof(temp_buf), "%.1f°", item.data.airTemperature);
  temperature->set_text(temp_buf);

  icon->set_text(item.data.symbolCode.get_weather_icon_str());
}