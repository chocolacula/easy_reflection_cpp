#pragma once

#include <string>

#include "er/expected.h"
#include "er/variable/var.h"

namespace rr {

struct String {

  String(std::string_view* str, bool is_const) : _var(str, is_const) {
  }

  String(std::string* str, bool is_const) : _var(str, is_const) {
  }

  std::string_view get() const {
    if (_var.type() == _shared_type) {
      return *static_cast<const std::string_view*>(_var.raw());
    }

    return std::string_view(*static_cast<const std::string*>(_var.raw()));
  }

  Expected<None> set(std::string_view value) {
    if (_var.is_const()) {
      return Error("Trying to set const value");
    }

    if (_var.type() == _shared_type) {
      *static_cast<std::string_view*>(_var.raw_mut()) = value;
    } else {
      *static_cast<std::string*>(_var.raw_mut()) = value;
    }
    return None();
  }

  Var var() const {
    return _var;
  }

 private:
  const Var _var;
  static const inline TypeId _shared_type = TypeId::get<std::string_view>();
};

}  // namespace rr