#include "smhi_client.hpp"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <iostream>
#include "./url/SMHIUrlGenerator.hpp"
#include "ArduinoJson/Document/JsonDocument.hpp"
#include "api/data/LatestMonthsStation.hpp"
#include "network/network.hpp"

bool fetch_from_url(const String& url, ArduinoJson::JsonDocument& outDoc) {
  if (!connect_wifi()) {
    std::cout << "WiFi not connected, cannot fetch SMHI data" << std::endl;
    return false;
  }
  std::cout << "Fetching url " << url.c_str() << std::endl;
  HTTPClient http;
  http.setTimeout(10000);
  http.begin(url);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    std::cout << "SMHI fetch failed, HTTP code: " << code << std::endl;
    return false;
  }

  String payload = http.getString();
  http.end();

  DeserializationError err = ArduinoJson::deserializeJson(outDoc, payload);
  if (err) {
    std::cout << "JSON parse error: " << err.c_str() << std::endl;
    return false;
  }
  return true;
}

bool fetch_seven_day_forecast(SevenDayForcastParameters params,
                              ForecastSevenDay& out) {
  String url = String(seven_day_forcast_data_url(params).c_str());

  std::cout << "Seven day forcast url " << url.c_str() << std::endl;
  ArduinoJson::JsonDocument outDoc;
  if (!fetch_from_url(url, outDoc)) {
    return false;
  }
  out.fromJson(outDoc);
  return true;
}
bool fetch_stations_latest_months(StationsHistoricalParameters params,
                                  LatestMonthsStations& out) {
  String url = String(stations_historical(params).c_str());
  if (url == "") {
    std::cout << "MeterologyCode is Unknown" << std::endl;
  }

  std::cout << "stations historical url " << url.c_str() << std::endl;
  ArduinoJson::JsonDocument outDoc;
  if (!fetch_from_url(url, outDoc)) {
    return false;
  }
  out.fromJson(outDoc, params.meterology);
  return true;
}
bool fetch_latest_months(StationsLatestMonthsParameters params,
                         ObservationSeries& out) {

  String url = String(latest_months_data(params).c_str());

  if (url == "") {
    std::cout << "MeterologyCode is Unknown" << std::endl;
  }
  std::cout << "Historical observations url " << url.c_str() << std::endl;
  ArduinoJson::JsonDocument outDoc;
  if (!fetch_from_url(url, outDoc)) {
    return false;
  }
  out.fromJson(outDoc);
  return true;
}
