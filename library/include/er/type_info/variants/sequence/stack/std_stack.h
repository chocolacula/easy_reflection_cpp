#pragma once

#include <stack>

#include "../err_helper.h"
#include "istack.h"
#include "stack_iterator.h"

namespace rr {

template <typename T>
struct StdStack : public IStack, public sequence::ErrHelper {
  StdStack() = delete;

  StdStack(std::stack<T>* stack, bool is_const)
      : _stack(stack),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_stack, TypeId::get(_stack), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto it = StackIterator<T>::begin(_stack); it != StackIterator<T>::end(_stack); ++it) {
      callback(Var(&(*it), nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {

    auto nested_type = TypeId::get<T>();

    for (auto it = StackIterator<T>::begin(_stack); it != StackIterator<T>::end(_stack); ++it) {
      callback(Var(&(*it), nested_type, _is_const));
    }
  }

  void clear() override {
    for (auto i = 0; i < _stack->size(); i++) {
      _stack->pop();
    }
  }

  size_t size() const override {
    return _stack->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set with type: {} to set<{}>",  //
                   value.type(), nested_type);
    }
    _stack->push(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _stack->pop();
  }

  Expected<Var> top() override {
    if (_stack->empty()) {
      return Error("The stack is empty");
    }

    return Var(&_stack->top(), TypeId::get<T>(), _is_const);
  };

 private:
  std::stack<T>* _stack;
  bool _is_const;
};

}  // namespace rr
