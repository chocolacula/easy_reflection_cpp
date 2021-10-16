#pragma once

#include <iomanip>
#include <sstream>

#include "ifloating.h"

namespace rr {

template <typename T>
struct Float : IFloating {
  Float(T* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  Var var() override {
    return Var(_value, _is_const);
  }

  size_t size() override {
    return sizeof(T);
  }

  double get() const override {
    return *_value;
  }

  Expected<None> set(double value) override {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (std::numeric_limits<T>::max() < value || -std::numeric_limits<T>::max() > value) {
      return Error("The value too big to set float variable");
    }

    *_value = value;
    return None();
  }

  std::string to_string(int precision) const override {
    std::stringstream stream;
    stream << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << *_value;

    return stream.str();
  }

 private:
  T* _value;
  const bool _is_const;
};

}  // namespace rr