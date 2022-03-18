#pragma once

#include <cstddef>

#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "er/variant/variant.h"
#include "istring.h"

namespace er {

template <typename T>
struct StdBasicString : IString {
  StdBasicString() = delete;

  StdBasicString(std::basic_string<T>* str, bool is_const) : _var(str, is_const) {
  }

  Expected<None> assign(Var var) override {
    if (var.type() != _var.type()) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(_var.type())));
    }
    _var = var;
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _var = Var(ptr, _var.type(), _var.is_const());
  }

  std::string_view get() const override {
    return *static_cast<const std::string*>(_var.raw());
  }

  Expected<None> set(std::string_view value) override {
    if (_var.is_const()) {
      return Error("Trying to set const value");
    }
    *static_cast<std::string*>(_var.raw_mut()) = std::string(value);
    return None();
  }

  Var var() const override {
    return _var;
  }

  bool owns_data() const override {
    return true;
  }

 private:
  Var _var;
};

}  // namespace er