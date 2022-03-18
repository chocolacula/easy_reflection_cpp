#pragma once

#include <iomanip>
#include <memory>
#include <sstream>

#include "./float.h"
#include "ifloating.h"

namespace er {

struct Floating final {
  Floating() = delete;

  template <typename T>
  Floating(T* value, bool is_const) {
    new (_mem) Float<T>(value, is_const);
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IFloating*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    return reinterpret_cast<IFloating*>(&_mem[0])->unsafe_assign(ptr);
  }

  Var var() {
    return reinterpret_cast<IFloating*>(&_mem[0])->var();
  }

  size_t size() {
    return reinterpret_cast<IFloating*>(&_mem[0])->size();
  }

  double get() const {
    return reinterpret_cast<const IFloating*>(&_mem[0])->get();
  }

  Expected<None> set(double value) {
    return reinterpret_cast<IFloating*>(&_mem[0])->set(value);
  }

  std::string to_string(int precision = 2) const {
    return reinterpret_cast<const IFloating*>(&_mem[0])->to_string(precision);
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of Float wrapper has the same sizeof()
  char _mem[sizeof(Float<float>)];
};

}  // namespace er