#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <utility>

#include "er/expected.h"
#include "er/reflection/type_name.h"
#include "er/tools/format.h"
#include "er/variable/var.h"
#include "fields.h"

namespace er {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Object {
  Object(Var var, const std::map<std::string_view, FieldDesc>* fields) : _fields(fields), _var(var) {
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
    _var.unsafe_assign(ptr);
  }

  Expected<FieldInfo> get_field(std::string_view name) {
    auto it = _fields->find(name);

    if (it != _fields->end()) {
      return FieldInfo(_var.raw(), &it->second);
    }
    return Error(format("There is no field with name: {}", name));
  }

  Fields get_fields(FieldAttributes access = FieldAttributes::kPublic) const {
    return Fields(_var.raw(), _fields, access);
  }

  Var var() {
    return _var;
  }

 private:
  const std::map<std::string_view, FieldDesc>* _fields;
  Var _var;
};

}  // namespace er