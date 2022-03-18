#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

#include "iinteger.h"
#include "int.h"

namespace er {

struct Integer final {
  Integer() = delete;

  template <typename T>
  Integer(T* value, bool is_const) {
    new (_mem) Int<T>(value, is_const);
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IInteger*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    reinterpret_cast<IInteger*>(&_mem[0])->unsafe_assign(ptr);
  }

  Var var() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->var();
  }

  size_t size() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->size();
  }

  bool is_signed() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->is_signed();
  }

  int64_t as_signed() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->as_signed();
  }

  uint64_t as_unsigned() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->as_unsigned();
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>,  //
                            Expected<None>>
  set(T value) {
    return reinterpret_cast<IInteger*>(&_mem[0])->set_signed(value);
  }

  template <typename T>
  typename std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>,  //
                            Expected<None>>
  set(T value) {
    return reinterpret_cast<IInteger*>(&_mem[0])->set_unsigned(value);
  }

  Expected<None> set_signed(int64_t value) {
    return reinterpret_cast<IInteger*>(&_mem[0])->set_signed(value);
  }

  Expected<None> set_unsigned(uint64_t value) {
    return reinterpret_cast<IInteger*>(&_mem[0])->set_unsigned(value);
  }

  std::string to_string() const {
    return reinterpret_cast<const IInteger*>(&_mem[0])->to_string();
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of Int wrapper has the same sizeof()
  char _mem[sizeof(Int<int>)];
};

}  // namespace er