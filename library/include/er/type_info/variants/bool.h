#pragma once

#include <algorithm>
#include <cctype>
#include <string_view>

#include "er/expected.h"
#include "er/variable/var.h"

namespace rr {

struct Bool {
  Bool(bool* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  bool get() const {
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
  const bool _is_const;
};

}  // namespace rr