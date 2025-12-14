#include <string>

namespace HttpCache {
    bool has(const std::string& key);
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value, unsigned long ttlSeconds);
    void clear();
    void clearExpired();
}