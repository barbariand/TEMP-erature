#include <string>
#include <vector>

struct HistoryEntry {
    std::string city;
    std::string parameter;
    unsigned long timestamp;
};

namespace HistoryStorage {
    std::vector<HistoryEntry> load();
    bool save(const std::vector<HistoryEntry>& entries);

    bool add(const std::string& city, const std::string& parameter);
    void clear();

    constexpr int MAX_HISTORY = 10;
}