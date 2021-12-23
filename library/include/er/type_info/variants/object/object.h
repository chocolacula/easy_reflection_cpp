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
  Object(Var var, std::map<std::string_view, FieldInfo>&& fields) : _fields(fields), _var(var) {
  }

  Expected<None> assign(Var var) {
    if (var.type() != _var.type()) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(_var.type())));
    }

    const auto* ptr = _var.raw();
    reassign_fields(ptr);
    _var = var;

    return None();
  }

  void unsafe_assign(void* ptr) {
    reassign_fields(ptr);
    _var = Var(ptr, _var.type(), false);
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

  inline void reassign_fields(const void* ptr) {
#pragma unroll 5
    for (auto&& kv : _fields) {
      auto& field = kv.second;

      auto origin = reinterpret_cast<uintptr_t>(_var.raw());
      auto delta = reinterpret_cast<uintptr_t>(field.var().raw()) - origin;

      auto new_origin = reinterpret_cast<uintptr_t>(ptr);

      kv.second = {Var(reinterpret_cast<void*>(new_origin + delta),
                       field.var().type(),  //
                       field.is_const()),   //
                   field.access()};
    }
  }
};

}  // namespace er