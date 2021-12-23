#pragma once

#include <vector>

#include "../err_helper.h"
#include "er/reflection/type_name.h"
#include "ivector.h"

namespace er {

template <typename T>
struct StdVector : public IVector, public sequence::ErrHelper {
  StdVector() = delete;

  StdVector(std::vector<T>* vector, bool is_const)
      : _vector(vector),  //
        _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_vector);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _vector = static_cast<std::vector<T>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _vector = static_cast<std::vector<T>*>(ptr);
  }

  Var own_var() const override {
    return Var(_vector, TypeId::get(_vector), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_vector) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_vector) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    for (auto&& entry : *_vector) {
      callback(&entry);
    }
  }

  void clear() override {
    _vector->clear();
  }

  size_t size() const override {
    return _vector->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return error("Trying to set value to vector<{}>",  //
                   value.type(), nested_type);
    }
    _vector->push_back(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _vector->pop_back();
  }

  Expected<Var> front() override {
    if (_vector->empty()) {
      return Error("The vector is empty");
    }
    return Var(&_vector->front(), TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    if (_vector->empty()) {
      return Error("The vector is empty");
    }
    return Var(&_vector->back(), TypeId::get<T>(), _is_const);
  };

  Expected<Var> at(size_t idx) override {
    if (idx >= _vector->size()) {
      return Error(format("Index: {} is out of array's size: {}", idx, _vector->size()));
    }

    return Var(&(*_vector)[idx], TypeId::get<T>(), _is_const);
  }

  Expected<Var> operator[](size_t idx) override {
    return at(idx);
  }

 private:
  std::vector<T>* _vector;
  bool _is_const;
};

}  // namespace er
