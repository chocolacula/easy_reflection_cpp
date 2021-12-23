#pragma once

#include <memory>

#include "std_stack.h"

namespace er {

struct Stack : public IStack {
  Stack() = delete;

  template <typename T>
  Stack(std::stack<T>* stack, bool is_const) : _stack(std::make_shared<StdStack<T>>(stack, is_const)) {
  }

  Expected<None> assign(Var var) override {
    return _stack->assign(var);
  }

  void unsafe_assign(void* ptr) override {
    _stack->unsafe_assign(ptr);
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

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    _stack->unsafe_for_each(callback);
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

}  // namespace er
