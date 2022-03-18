#pragma once

#include <algorithm>
#include <cctype>
#include <string_view>

#include "er/expected.h"
#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "er/type_id.h"
#include "er/variable/var.h"

namespace er {

struct Bool {
  Bool(bool* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  Expected<None> assign(Var var) {
    auto t = TypeId::get<bool>();
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _value = const_cast<bool*>(static_cast<const bool*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) {
    _value = static_cast<bool*>(ptr);
    _is_const = false;
  }

  [[nodiscard]] bool get() const {
    return *_value;
  }

  Expected<None> set(bool value) {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    *_value = value;
    return None();
  }

  Var var() {
    return Var(_value, _is_const);
  }

  std::string_view to_string() const {
    return *_value ? "true" : "false";
  }

 private:
  bool* _value;
  bool _is_const;
};

}  // namespace er