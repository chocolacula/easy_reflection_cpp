#pragma once

#include <memory>

#include "std_queue.h"

namespace rr {

struct Queue : public IQueue {
  Queue() = delete;

  template <typename T>
  explicit Queue(std::queue<T>* queue, bool is_const) : _queue(std::make_shared<StdQueue<T>>(queue, is_const)) {
  }

  Var own_var() const override {
    return _queue->own_var();
  }

  TypeId nested_type() const override {
    return _queue->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const override {
    static_cast<const ISequence*>(_queue.get())->for_each(callback);
  }

  void for_each(std::function<void(Var)> callback) override {
    _queue->for_each(callback);
  }

  void clear() override {
    _queue->clear();
  }

  size_t size() const override {
    return _queue->size();
  }

  Expected<None> push(Var value) override {
    return _queue->push(value);
  }

  void pop() override {
    _queue->pop();
  }

  Expected<Var> front() override {
    return _queue->front();
  };

  Expected<Var> back() override {
    return _queue->back();
  };

 private:
  std::shared_ptr<IQueue> _queue;
};

}  // namespace rr
