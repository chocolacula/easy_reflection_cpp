#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

#include "iinteger.h"
#include "int.h"

namespace er {

struct Integer : IInteger {
  Integer() = delete;

  template <typename T>
  Integer(T* value, bool is_const) : _integer(std::make_shared<Int<T>>(value, is_const)) {
  }

  Var var() const override {
    return _integer->var();
  }

  size_t size() const override {
    return _integer->size();
  }

  bool is_signed() const override {
    return _integer->is_signed();
  }

  int64_t as_signed() const override {
    return _integer->as_signed();
  }

  uint64_t as_unsigned() const override {
    return _integer->as_unsigned();
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,  //
                            Expected<None>>
  set(T value) {
    return _integer->set_signed(value);
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,  //
                            Expected<None>>
  set(T value) {
    return _integer->set_unsigned(value);
  }

  Expected<None> set_signed(int64_t value) override {
    return _integer->set_signed(value);
  }

  Expected<None> set_unsigned(uint64_t value) override {
    return _integer->set_unsigned(value);
  }

  std::string to_string() const override {
    return _integer->to_string();
  }

 private:
  std::shared_ptr<IInteger> _integer;
};

}  // namespace er