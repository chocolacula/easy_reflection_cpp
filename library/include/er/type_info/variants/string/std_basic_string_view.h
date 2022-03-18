#pragma once

#include <cstddef>

#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "er/variant/variant.h"
#include "istring.h"

namespace er {

template <typename T>
struct StdBasicStringView : IString {
  StdBasicStringView() = delete;

  StdBasicStringView(std::basic_string_view<T>* str) : _var(str, true) {
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
    _var.unsafe_assign(ptr);
  }

  std::string_view get() const override {
    return *static_cast<const std::string_view*>(_var.raw());
  }

  Expected<None> set(std::string_view value) override {
    return Error("Trying to set const value");
    // keep it as possible implementation
    // if (_var.is_const()) {
    //   return Error("Trying to set const value");
    // }
    // *static_cast<std::string_view*>(_var.raw_mut()) = value;
    // return None();
  }

  Var var() const override {
    return _var;
  }

 private:
  Var _var;
};

}  // namespace er