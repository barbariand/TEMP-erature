#include "ForcastUiItem.hpp"
void ForcastUIItem::update_data(const TimeSeriesItem& item) {
  date->set_text(item.time.toString("%A, %d %B"));
  char temp_buf[16];
  snprintf(temp_buf, sizeof(temp_buf), "%.1f°C", item.data.airTemperature);

  temperature->set_text(temp_buf);
  icon->set_text(item.data.symbolCode.get_weather_icon_str());
}
