#pragma once

#include <iomanip>
#include <limits>
#include <sstream>

#include "er/reflection/type_name.h"
#include "ifloating.h"

namespace er {

template <typename T>
struct Float : IFloating {
  Float(T* value, bool is_const) : _value(value), _is_const(is_const) {
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
  bool _is_const;
};

}  // namespace er