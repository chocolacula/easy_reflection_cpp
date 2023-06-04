#pragma once

#include <queue>

namespace er {

template <typename T>
struct QueueIterator : public std::queue<T> {
  static decltype(auto) begin(std::queue<T>* queue) {
    return (queue->*&QueueIterator::c).begin();
  }

  static decltype(auto) end(std::queue<T>* queue) {
    return (queue->*&QueueIterator::c).end();
  }

  template <typename SameT>
  friend struct StdQueue;
};

}  // namespace er
