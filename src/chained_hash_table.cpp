#include "chained_hash_table.hpp"

#include <stdexcept>

namespace lab {

std::size_t ChainedHashTable::bucket_index(const std::string& key) const {
    std::size_t h = 0;
    for (const char ch : key) {
        const unsigned char c = static_cast<unsigned char>(ch);
        h = (31 * h + c) % kBucketCount;
    }
    return h;
}

void ChainedHashTable::put(const std::string& key, int value) {
    auto& chain = buckets_[bucket_index(key)];
    for (Entry& entry : chain) {
        if (entry.key == key) {
            entry.value = value;  // duplicate key: update, size unchanged
            return;
        }
    }
    chain.push_back(Entry{key, value});
    ++size_;
}

std::optional<int> ChainedHashTable::find(const std::string& key) const {
    const auto& chain = buckets_[bucket_index(key)];
    for (const Entry& entry : chain) {
        if (entry.key == key) {
            return entry.value;
        }
    }
    return std::nullopt;
}

bool ChainedHashTable::erase(const std::string& key) {
    auto& chain = buckets_[bucket_index(key)];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->key == key) {
            chain.erase(it);
            --size_;
            return true;
        }
    }
    return false;
}

}  // namespace lab
