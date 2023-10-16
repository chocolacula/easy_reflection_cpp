#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

#include "iinteger.h"
#include "int.h"

namespace er {

struct Integer {
  Integer() = delete;

  template <typename T>
  Integer(T* value, bool is_const) {
    new (_mem) Int<T>(value, is_const);
  }

  Expected<None> assign(Var var) {
    return impl()->assign(var);
  }

  void unsafe_assign(void* ptr) {
    impl()->unsafe_assign(ptr);
  }

  Var var() const {
    return impl()->var();
  }

  size_t size() const {
    return impl()->size();
  }

  bool is_signed() const {
    return impl()->is_signed();
  }

  int64_t as_signed() const {
    return impl()->as_signed();
  }

  uint64_t as_unsigned() const {
    return impl()->as_unsigned();
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,  //
                            Expected<None>>
  set(T value) {
    return impl()->set_signed(value);
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,  //
                            Expected<None>>
  set(T value) {
    return impl()->set_unsigned(value);
  }

  Expected<None> set_signed(int64_t value) {
    return impl()->set_signed(value);
  }

  Expected<None> set_unsigned(uint64_t value) {
    return impl()->set_unsigned(value);
  }

 private:
  char _mem[sizeof(Int<int>)];

  inline const IInteger* impl() const {
    return reinterpret_cast<const IInteger*>(&_mem[0]);
  }

  inline IInteger* impl() {
    return reinterpret_cast<IInteger*>(&_mem[0]);
  }
};

}  // namespace er
