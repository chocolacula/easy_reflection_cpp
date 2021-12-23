#pragma once

#include <string>

#include "er/expected.h"
#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "er/variable/var.h"
#include "er/variant/variant.h"

namespace er {

struct String {

  String(std::string_view* str, bool is_const) : _var(str, is_const) {
  }

  String(std::string* str, bool is_const) : _var(str, is_const) {
  }

  Expected<None> assign(Var var) {
    if (var.type() != _var.type()) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(_var.type())));
    }

    _var = var;
    return None();
  }

  void unsafe_assign(void* ptr) {
    _var = Var(ptr, _var.type(), _var.is_const());
  }

  std::string_view get() const {
    if (_var.type() == _shared_type) {
      return *static_cast<const std::string_view*>(_var.raw());
    }

    return std::string_view(*static_cast<const std::string*>(_var.raw()));
  }

  // TODO setting std::string_view is BAD idea especially via serialization
  Expected<None> set(std::string_view value) {
    if (_var.is_const()) {
      return Error("Trying to set const value");
    }

    if (_var.type() == _shared_type) {
      *static_cast<std::string_view*>(_var.raw_mut()) = value;
    } else {
      *static_cast<std::string*>(_var.raw_mut()) = std::string(value);
    }
    return None();
  }

  Var var() const {
    return _var;
  }

 private:
  Var _var;
  static const inline TypeId _shared_type = TypeId::get<std::string_view>();
};

}  // namespace er