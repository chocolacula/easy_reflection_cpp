#pragma once

#include <set>

#include "../err_helper.h"
#include "er/reflection/type_name.h"
#include "iset.h"

namespace er {

template <typename T>
struct StdSet : public ISet, public sequence::ErrHelper {
  StdSet() = delete;

  StdSet(std::set<T>* set, bool is_const)
      : _set(set),  //
        _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_set);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _set = static_cast<std::set<T>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _set = static_cast<std::set<T>*>(ptr);
    _is_const = false;
  }

  Var own_var() const override {
    return Var(_set, TypeId::get(_set), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    for (auto&& entry : *_set) {
      // const values
      callback(Var(&entry));
    }
  }

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    for (auto&& entry : *_set) {
      // const values
      callback(const_cast<T*>(&entry));
    }
  }

  void clear() override {
    _set->clear();
  }

  size_t size() const override {
    return _set->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set with type: {} to set<{}>",  //
                   value.type(), nested_type);
    }
    _set->insert(*static_cast<const T*>(value.raw()));
    return None();
  }

  Expected<None> remove(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Cannot remove value with type: {} from set<{}>",  //
                   value.type(), nested_type);
    }
    _set->erase(*static_cast<const T*>(value.raw()));
    return None();
  }

  bool contains(Var value) override {
    auto it = _set->find(*static_cast<const T*>(value.raw()));
    return it != _set->end();
  }

 private:
  std::set<T>* _set;
  bool _is_const;
};

}  // namespace er
