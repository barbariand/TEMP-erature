#include "history_storage.hpp"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <FS.h>

using namespace std;

static const char* HISTORY_PATH = "/history.json";

namespace HistoryStorage {

    std::vector<HistoryEntry> load() {
        std::vector<HistoryEntry> list;

        if (!SPIFFS.begin(true)) {
            return list;
        }

        if (!SPIFFS.exists(HISTORY_PATH)) {
            return list;
        }

        File f = SPIFFS.open(HISTORY_PATH, FILE_READ);
        if (!f) return list;

        size_t size = f.size();
        std::unique_ptr<char[]> buf(new char[size + 1]);
        f.readBytes(buf.get(), size);
        buf[size] = '\0';
        f.close();

        JsonDocument doc;
        auto err = deserializeJson(doc, buf.get());
        if (err) return list;

        if (!doc.is<JsonArray>()) return list;

        for (JsonObject obj : doc.as<JsonArray>()) {
            HistoryEntry e;
            e.city = obj["city"].as<const char*>();
            e.parameter = obj["parameter"].as<const char*>();
            e.timestamp = obj["timestamp"] | 0;
            list.push_back(e);
        }

        return list;
    }

    bool save(const std::vector<HistoryEntry>& entries) {

        if (!SPIFFS.begin(true)) {
            return false;
        }

        JsonDocument doc;
        JsonArray arr = doc.to<JsonArray>();

        for (const auto& e : entries) {
            JsonObject o = arr.add<JsonObject>();
            o["city"] = e.city.c_str();
            o["parameter"] = e.parameter.c_str();
            o["timestamp"] = e.timestamp;
        }

        File f = SPIFFS.open(HISTORY_PATH, FILE_WRITE);

        if (!f) 
            return false;

        bool ok = serializeJson(doc, f) > 0;
        f.close();

        return ok;
    }

    bool add(const std::string& city, const std::string& parameter) {
        auto list = load();

        // check for duplicates
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->city == city && it->parameter == parameter) {
                list.erase(it);
                break;
            }
        }

        HistoryEntry e;
        e.city = city;
        e.parameter = parameter;
        e.timestamp = time(NULL);

        list.insert(list.begin(), e);

        // resize if above max history
        if (list.size() > MAX_HISTORY) {
            list.resize(MAX_HISTORY);
        }

        return save(list);
    }

    void clear() {
        if (!SPIFFS.begin(true)) return;
        if (SPIFFS.exists(HISTORY_PATH)) {
            SPIFFS.remove(HISTORY_PATH);
        }
    }

}