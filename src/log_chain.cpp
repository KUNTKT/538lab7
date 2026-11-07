#include "log_chain.hpp"

#include <stdexcept>

namespace lab {

void LogChain::append(const std::string& record) {
    const crypto::Digest previous = final_digest();
    entries_.push_back(LogEntry{record, crypto::hash_record(previous, record)});
}

crypto::Digest LogChain::final_digest() const {
    if (entries_.empty()) {
        return initial_digest();
    }
    return entries_.back().digest;
}

bool LogChain::verify(const std::vector<LogEntry>& entries,
                      const crypto::Digest& trusted_final) {
    crypto::Digest current = initial_digest();
    for (const LogEntry& entry : entries) {
        current = crypto::hash_record(current, entry.record);
        if (current != entry.digest) {
            return false;
        }
    }
    return current == trusted_final;
}

}  // namespace lab
