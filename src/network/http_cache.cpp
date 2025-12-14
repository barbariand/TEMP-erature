#include "http_cache.hpp"
#include <unordered_map>
#include <Arduino.h>

struct CacheEntry {
    std::string entry;
    unsigned long timestamp;
    unsigned long ttl;
};

namespace HttpCache {
    static std::unordered_map<std::string, CacheEntry> cache;

    bool isExpired(const CacheEntry& e) {
        unsigned long now = time(NULL);
        unsigned long ageMs = now - e.timestamp;
        return ageMs > (e.ttl * 1000);
    }

    bool has(const std::string& key) {
        auto it = cache.find(key);
        if (it == cache.end()) return false;
        return !isExpired(it->second);
    }

    std::string get(const std::string& key) {
        if (!has(key)) return "";
        return cache[key].entry;
    }

    void set(const std::string& key, const std::string& entry, unsigned long ttlSeconds) {
        CacheEntry e;
        e.entry = entry;
        e.timestamp = time(NULL);
        e.ttl = ttlSeconds;
        cache[key] = e;
    }

    void clear() {
        cache.clear();
    }

    void clearExpired() {
        for (auto it = cache.begin(); it != cache.end(); /* dont iterate */) {
            if (isExpired(it->second)) {
                it = cache.erase(it);
            } else {
                ++it; //iterate
            }
        }
    }
}