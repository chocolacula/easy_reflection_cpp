#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "iinteger.h"

namespace er {

template <typename T>
struct Int : IInteger {
  Int(T* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_value);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _value = static_cast<T*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _value = static_cast<T*>(ptr);
    _is_const = false;
  }

  Var var() const override {
    return Var(_value, _is_const);
  }

  size_t size() const override {
    return sizeof(T);
  }

  bool is_signed() const override {
    return std::is_signed_v<T>;
  }

  int64_t as_signed() const override {
    return *_value;
  }

  uint64_t as_unsigned() const override {
    return *_value;
  }

  Expected<None> set_signed(int64_t value) override {
    if (_is_const) {
      return Error("Trying to set const value");
    }

    if (std::numeric_limits<T>::max() < value || std::numeric_limits<T>::min() > value) {
      return Error(format("The value is too big to fit {} byte variable", sizeof(*_value)));
    }

    if (value < 0 && !is_signed()) {
      return Error("Cannot assign negative value to unsigned");
    }

    *_value = value;
    return None();
  }

  Expected<None> set_unsigned(uint64_t value) override {
    if (_is_const) {
      return Error("Trying to set const value");
    }

    if (std::numeric_limits<T>::max() < value || std::numeric_limits<T>::min() > value) {
      return Error(format("The value is too big to fit {} byte variable", sizeof(*_value)));
    }

    *_value = value;
    return None();
  }

  std::string to_string() const override {
    return std::to_string(*_value);
  }

 private:
  T* _value;
  bool _is_const;
};

}  // namespace er