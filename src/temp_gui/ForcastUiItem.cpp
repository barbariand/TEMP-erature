#include "ForcastUiItem.hpp"
void ForcastUIItem::update_data(const TimeSeriesItem& item) {
  date->set_text(item.time.toString("%A, %d %B"));
  temperature->set_text(std::to_string(item.data.airTemperature) + "°C");
  icon->set_text(item.data.symbolCode.get_weather_icon_str());
}
