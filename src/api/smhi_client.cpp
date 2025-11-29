#include "smhi_client.hpp"
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "network/network.hpp"

static String seven_day_url(double lon, double lat) {
  // SMHI point forecast endpoint (best-effort)
  char buf[256];
  // Use 6 fractional digits for coords
  snprintf(buf, sizeof(buf), "https://opendata-download-metfcst.smhi.se/api/category/pmp3g/version/2/geotype/point/lon/%.6f/lat/%.6f/data.json", lon, lat);
  return String(buf);
}

bool fetch_from_url(const String& url, ArduinoJson::DynamicJsonDocument& outDoc, unsigned long timeout_ms) {
  if (!connect_wifi()) {
    Serial.println("WiFi not connected, cannot fetch SMHI data");
    return false;
  }

  HTTPClient http;
  http.setTimeout(timeout_ms);
  http.begin(url);
  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    Serial.printf("SMHI fetch failed, HTTP code: %d\n", code);
    return false;
  }

  String payload = http.getString();
  http.end();

  DeserializationError err = deserializeJson(outDoc, payload);
  if (err) {
    Serial.printf("JSON parse error: %s\n", err.c_str());
    return false;
  }
  return true;
}

bool fetch_seven_day_forecast(double lon, double lat, ArduinoJson::DynamicJsonDocument& outDoc, unsigned long timeout_ms) {
  String url = seven_day_url(lon, lat);
  return fetch_from_url(url, outDoc, timeout_ms);
}

bool fetch_latest_months(double lon, double lat, ArduinoJson::DynamicJsonDocument& outDoc, unsigned long timeout_ms) {
  // Best-effort: try same endpoint for now (project may add station-based obs later)
  return fetch_seven_day_forecast(lon, lat, outDoc, timeout_ms);
}
