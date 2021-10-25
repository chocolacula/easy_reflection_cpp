#pragma once

#include "er/tools/format.h"
#include "iarray.h"

namespace er {

template <typename T, size_t size_v>
struct CArray final : public IArray {
  CArray() = delete;

  CArray(T (*array)[size_v], bool is_const) : _array(reinterpret_cast<T*>(array)), _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_array, TypeId::get<T[size_v]>(), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&(_array[i]), nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&(_array[i]), nested_type, _is_const));
    }
  }

  size_t size() const override {
    return size_v;
  }

  Expected<Var> front() override {
    return Var(_array, TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    return Var(&(_array[size_v - 1]), TypeId::get<T>(), _is_const);
  };

  Expected<Var> at(size_t idx) override {
    if (idx >= size_v) {
      return Error(format("Index: {} is out of array's size: {}", idx, size_v));
    }

    return Var(&(_array[idx]), TypeId::get<T>(), _is_const);
  }

  Expected<Var> operator[](size_t idx) override {
    return at(idx);
  }

  Expected<None> fill(Var filler) override {
    auto f = filler.rt_cast<T>();

    return f.template match(
        [this](T* ptr) -> Expected<None> {  //
          for (auto i = 0; i < size_v; i++) {
            _array[i] = *ptr;
          }
          return None();
        },
        [](Error err) -> Expected<None> {  //
          return err;
        });
  }

 private:
  T* _array;
  bool _is_const;
};

}  // namespace er
