#include "settings_storage.hpp"
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include "api/parameters/MeterologyCode.hpp"
using namespace ArduinoJson;
using namespace std;

static const char* SETTINGS_PATH = "/settings.json";

namespace SettingsStorage {

Settings load() {
  Settings s;
  if (!LittleFS.begin(true))
    return s;
  if (!LittleFS.exists(SETTINGS_PATH))
    return s;

  File f = LittleFS.open(SETTINGS_PATH, FILE_READ);
  if (!f)
    return s;

  size_t size = f.size();
  std::unique_ptr<char[]> buf(new char[size + 1]);
  f.readBytes(buf.get(), size);
  buf[size] = '\0';
  f.close();

  JsonDocument doc;
  auto err = deserializeJson(doc, buf.get());
  if (err)
    return s;

  if (doc.containsKey("city"))
    s.city = doc["city"].as<const char*>();
  if (doc.containsKey("parameter"))
    s.parameter = MeterologyCode(doc["parameter"].as<int>());

  return s;
}

bool save(const Settings& s) {
  if (!LittleFS.begin(true))
    return false;

  JsonDocument doc;
  doc["city"] = s.city.c_str();
  doc["parameter"] = (int)s.parameter.value;

  File f = LittleFS.open(SETTINGS_PATH, FILE_WRITE);
  if (!f)
    return false;

  if (serializeJson(doc, f) == 0) {
    f.close();
    return false;
  }
  f.close();
  return true;
}

}  // namespace SettingsStorage
