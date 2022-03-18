#pragma once

#include <deque>

#include "../err_helper.h"
#include "er/reflection/type_name.h"
#include "ilist.h"

namespace er {

template <typename T>
struct StdDeque : public IList, public sequence::ErrHelper {
  StdDeque() = delete;

  StdDeque(std::deque<T>* deque, bool is_const)
      : _deque(deque),  //
        _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_deque);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _deque = static_cast<std::deque<T>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _deque = static_cast<std::deque<T>*>(ptr);
    _is_const = false;
  }

  Var own_var() const override {
    return Var(_deque, TypeId::get(_deque), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    const auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_deque) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    const auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_deque) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    for (auto&& entry : *_deque) {
      callback(&entry);
    }
  }

  void clear() override {
    _deque->clear();
  }

  size_t size() const override {
    return _deque->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set with type: {} to deque<{}>",  //
                   value.type(), nested_type);
    }
    _deque->push_back(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _deque->pop_back();
  }

  void push_front(Var element) override {
    _deque->push_front(*static_cast<const T*>(element.raw()));
  }

  void pop_front() override {
    _deque->pop_front();
  }

  Expected<Var> front() override {
    if (_deque->empty()) {
      return Error("The list is empty");
    }
    return Var(&_deque->front(), TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    if (_deque->empty()) {
      return Error("The list is empty");
    }
    return Var(&_deque->back(), TypeId::get<T>(), _is_const);
  };

 private:
  std::deque<T>* _deque;
  bool _is_const;
};

}  // namespace er
