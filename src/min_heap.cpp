#include "min_heap.hpp"

#include <stdexcept>
#include <utility>

namespace lab {

void MinHeap::build(const std::vector<int>& values) {
    data_ = values;
    // Sift down every internal node, from the last one back to the root.
    // Count i from n/2 down to 1 and use i - 1 to avoid unsigned underflow.
    for (std::size_t i = data_.size() / 2; i > 0; --i) {
        sift_down(i - 1);
    }
}

void MinHeap::push(int value) {
    data_.push_back(value);
    sift_up(data_.size() - 1);
}

int MinHeap::pop() {
    if (data_.empty()) {
        throw std::out_of_range("MinHeap::pop on empty heap");
    }
    const int result = data_.front();
    data_.front() = data_.back();
    data_.pop_back();
    if (!data_.empty()) {
        sift_down(0);
    }
    return result;
}

int MinHeap::top() const {
    if (data_.empty()) {
        throw std::out_of_range("MinHeap::top on empty heap");
    }
    return data_.front();
}

void MinHeap::sift_up(std::size_t index) {
    while (index > 0) {
        const std::size_t parent = (index - 1) / 2;
        if (data_[parent] <= data_[index]) {
            break;
        }
        std::swap(data_[parent], data_[index]);
        index = parent;
    }
}

void MinHeap::sift_down(std::size_t index) {
    const std::size_t n = data_.size();
    while (true) {
        const std::size_t left = 2 * index + 1;
        if (left >= n) {
            break;
        }
        const std::size_t right = left + 1;
        std::size_t smallest = left;
        if (right < n && data_[right] < data_[left]) {
            smallest = right;
        }
        if (data_[index] <= data_[smallest]) {
            break;
        }
        std::swap(data_[index], data_[smallest]);
        index = smallest;
    }
}

}  // namespace lab
