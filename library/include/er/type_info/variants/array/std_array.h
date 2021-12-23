#pragma once

#include <vector>

#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "iarray.h"

namespace er {

template <typename T, size_t size_v>
struct StdArray final : public IArray {
  StdArray() = delete;

  StdArray(std::array<T, size_v>* array, bool is_const) : _array(array), _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_array);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _array = static_cast<std::array<T, size_v>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _array = static_cast<std::array<T, size_v>*>(ptr);
    _is_const = false;
  }

  Var own_var() const override {
    return Var(_array, TypeId::get(_array), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&_array[i], nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&(*_array)[i]));
    }
  }

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    for (auto i = 0; i < size_v; ++i) {
      callback(&(*_array)[i]);
    }
  }

  size_t size() const override {
    return size_v;
  }

  Expected<Var> front() override {
    return Var(&(*_array)[0], TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    return Var(&(*_array)[size_v - 1], TypeId::get<T>(), _is_const);
  };

  Expected<Var> at(size_t idx) override {
    if (idx >= size_v) {
      return Error(format("Index: {} is out of array's size: {}", idx, size_v));
    }

    return Var(&(*_array)[idx], TypeId::get<T>(), _is_const);
  }

  Expected<Var> operator[](size_t idx) override {
    return at(idx);
  }

  Expected<None> fill(Var filler) override {
    auto f = filler.rt_cast<T>();

    return f.template match_move(
        [this](T* ptr) -> Expected<None> {  //
          _array->fill(*ptr);
          return None();
        },
        [](Error&& err) -> Expected<None> {  //
          return err;
        });
  }

 private:
  std::array<T, size_v>* _array;
  bool _is_const;
};

}  // namespace er
