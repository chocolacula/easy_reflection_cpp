#pragma once

#include <queue>

template <typename T>
struct QueueIterator : public std::queue<T> {
  static typename decltype(QueueIterator::c)::iterator begin(std::queue<T>* queue) {
    return (queue->*&QueueIterator::c).begin();
  }

  static typename decltype(QueueIterator::c)::iterator end(std::queue<T>* queue) {
    return (queue->*&QueueIterator::c).end();
  }

  template <typename SameT>
  friend class StdQueue;
};