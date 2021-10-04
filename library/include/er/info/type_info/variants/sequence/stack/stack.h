#pragma once

#include "std_stack.h"

namespace rr {

struct Stack : public IStack {
  Stack() = delete;

  template <typename T>
  explicit Stack(std::stack<T>* stack, bool is_const) : _stack(std::make_shared<StdStack<T>>(stack, is_const)) {
  }

  Var own_var() const override {
    return _stack->own_var();
  }

  TypeId nested_type() const override {
    return _stack->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const override {
    static_cast<const ISequence*>(_stack.get())->for_each(callback);
  }

  void for_each(std::function<void(Var)> callback) override {
    _stack->for_each(callback);
  }

  void clear() override {
    _stack->clear();
  }

  size_t size() const override {
    return _stack->size();
  }

  Expected<None> push(Var value) override {
    return _stack->push(value);
  }

  void pop() override {
    _stack->pop();
  }

  Expected<Var> top() override {
    return _stack->top();
  };

 private:
  std::shared_ptr<IStack> _stack;
};

}  // namespace rr
