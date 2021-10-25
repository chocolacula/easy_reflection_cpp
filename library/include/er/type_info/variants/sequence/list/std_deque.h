#pragma once

#include <deque>

#include "../err_helper.h"
#include "ilist.h"

namespace er {

template <typename T>
struct StdDeque : public IList, public sequence::ErrHelper {
  StdDeque() = delete;

  StdDeque(std::deque<T>* deque, bool is_const)
      : _deque(deque),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_deque, TypeId::get(_deque), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_deque) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_deque) {
      callback(Var(&entry, nested_type, _is_const));
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
