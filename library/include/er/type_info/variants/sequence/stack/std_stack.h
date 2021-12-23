#pragma once

#include <stack>

#include "../err_helper.h"
#include "er/reflection/type_name.h"
#include "istack.h"
#include "stack_iterator.h"

namespace er {

template <typename T>
struct StdStack : public IStack, public sequence::ErrHelper {
  StdStack() = delete;

  StdStack(std::stack<T>* stack, bool is_const)
      : _stack(stack),  //
        _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_stack);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _stack = static_cast<std::stack<T>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _stack = static_cast<std::stack<T>*>(ptr);
    _is_const = false;
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

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    for (auto it = StackIterator<T>::begin(_stack); it != StackIterator<T>::end(_stack); ++it) {
      callback(&(*it));
    }
  }

  void clear() override {
    while (!_stack->empty()) {
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

}  // namespace er
