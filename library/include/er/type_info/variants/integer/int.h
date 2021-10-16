#pragma once

#include <cstdint>
#include <type_traits>

#include "er/tools/format.h"
#include "iinteger.h"

namespace rr {

template <typename T>
struct Int : IInteger {
  Int(T* value, bool is_const) : _value(value), _is_const(is_const) {
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

  int64_t get() const override {
    return *_value;
  }

  Expected<None> set(int64_t value) override {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (std::numeric_limits<T>::max() < value || std::numeric_limits<T>::min() > value) {
      return Error(format("The value is too big to fit {} byte variable", sizeof(*_value)));
    }

    auto is_signed = std::is_signed_v<T>;
    if (value < 0 && !is_signed) {
      return Error("Cannot assign negative value to unsigned");
    }

    *_value = value;
    return None();
  }

  std::string to_string() const override {
    return std::to_string(*_value);
  }

 private:
  T* _value;
  const bool _is_const;
};

}  // namespace rr