#pragma once

#include <cstdint>
#include <memory>

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

  int64_t get() const override {
    return _integer->get();
  }

  Expected<None> set(int64_t value) override {
    return _integer->set(value);
  }

  std::string to_string() const override {
    return _integer->to_string();
  }

 private:
  std::shared_ptr<IInteger> _integer;
};

}  // namespace er