#pragma once

#include <memory>

#include "float.h"
#include "ifloating.h"

namespace er {

struct Floating final {
  Floating() = delete;

  template <typename T>
  Floating(T* value, bool is_const) {
    new (_mem) Float<T>(value, is_const);
  }

  Expected<None> assign(Var var) {
    return impl()->assign(var);
  }

  void unsafe_assign(void* ptr) {
    return impl()->unsafe_assign(ptr);
  }

  Var var() {
    return impl()->var();
  }

  size_t size() {
    return impl()->size();
  }

  double get() const {
    return impl()->get();
  }

  Expected<None> set(double value) {
    return impl()->set(value);
  }

 private:
  char _mem[sizeof(Float<double>)];

  inline const IFloating* impl() const {
    return reinterpret_cast<const IFloating*>(&_mem[0]);
  }

  inline IFloating* impl() {
    return reinterpret_cast<IFloating*>(&_mem[0]);
  }
};

}  // namespace er
