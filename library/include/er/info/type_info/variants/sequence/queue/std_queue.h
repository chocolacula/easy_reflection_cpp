#pragma once

#include <queue>

#include "../err_helper.h"
#include "iqueue.h"
#include "queue_iterator.h"

namespace rr {

template <typename T>
struct StdQueue : public IQueue, public sequence::ErrHelper {
  StdQueue() = delete;

  explicit StdQueue(std::queue<T>* queue, bool is_const)
      : _queue(queue),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_queue, TypeId::get(_queue), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto it = QueueIterator<T>::begin(_queue); it != QueueIterator<T>::end(_queue); ++it) {
      callback(Var(&(*it), nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto it = QueueIterator<T>::begin(_queue); it != QueueIterator<T>::end(_queue); ++it) {
      callback(Var(&(*it), nested_type, _is_const));
    }
  }

  void clear() override {
    for (auto i = 0; i < _queue->size(); i++) {
      _queue->pop();
    }
  }

  size_t size() const override {
    return _queue->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set with type: {} to queue<{}>",  //
                   value.type(), nested_type);
    }
    _queue->push(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _queue->pop();
  }

  Expected<Var> front() override {
    return Var(&_queue->front());
  };

  Expected<Var> back() override {
    return Var(&_queue->back());
  };

 private:
  std::queue<T>* _queue;
  bool _is_const;
};

}  // namespace rr
