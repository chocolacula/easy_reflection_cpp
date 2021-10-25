#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <utility>

#include "er/expected.h"
#include "er/tools/format.h"
#include "er/variable/var.h"
#include "fields.h"

namespace er {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Object {
  Object(Var var, std::map<std::string_view, FieldInfo>&& fields) : _fields(fields), _var(var) {
  }

  Expected<Var> get_field(std::string_view name) {

    auto it = _fields.find(name);
    if (it != _fields.end()) {
      return it->second.var();
    }
    return Error(format("There is no field with name: {}", name));
  }

  Fields get_fields(Access access = Access::kPublic) const {
    return Fields(&_fields, access);
  }

  Var var() {
    return _var;
  }

 private:
  std::map<std::string_view, FieldInfo> _fields;
  Var _var;
};

}  // namespace er