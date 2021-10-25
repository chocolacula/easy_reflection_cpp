#pragma once

#include <list>

#include "../err_helper.h"
#include "ilist.h"

namespace er {

template <typename T>
struct StdList : public IList, public sequence::ErrHelper {
  StdList() = delete;

  StdList(std::list<T>* list, bool is_const)
      : _list(list),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_list, TypeId::get(_list), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_list) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_list) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void clear() override {
    _list->clear();
  }

  size_t size() const override {
    return _list->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set with type: {} to list<{}>",  //
                   value.type(), nested_type);
    }
    _list->push_back(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _list->pop_back();
  }

  void push_front(Var element) override {
    _list->push_front(*static_cast<const T*>(element.raw()));
  }

  void pop_front() override {
    _list->pop_front();
  }

  Expected<Var> front() override {
    if (_list->empty()) {
      return Error("The list is empty");
    }
    return Var(&_list->front(), TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    if (_list->empty()) {
      return Error("The list is empty");
    }
    return Var(&_list->back(), TypeId::get<T>(), _is_const);
  };

 private:
  std::list<T>* _list;
  bool _is_const;
};

}  // namespace er
