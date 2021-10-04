#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <utility>

#include "../../field_info.h"
#include "er/expected.h"
#include "er/variable/var.h"

namespace rr {

enum class FieldFlags : uint8_t {
  kDefault = 0,  // public nonstatic fields
  kNonPublic = 0b01,
  kStatic = 0b10
};

inline FieldFlags operator|(FieldFlags lhs, FieldFlags rhs) {
  return static_cast<FieldFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline FieldFlags operator&(FieldFlags lhs, FieldFlags rhs) {
  return static_cast<FieldFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

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

  std::vector<std::pair<std::string_view, Var>> get_fields(FieldFlags flags = FieldFlags::kDefault) const {

    auto with_non_public = (flags & FieldFlags::kNonPublic) != FieldFlags::kDefault;
    auto with_static = (flags & FieldFlags::kStatic) != FieldFlags::kDefault;

    std::vector<std::pair<std::string_view, Var>> fields;

    for (auto&& f : _fields) {

      if (!f.second.is_static()) {
        if (f.second.is_public()) {
          fields.emplace_back(f.first, f.second.var());
        } else {
          if (with_non_public) {
            fields.emplace_back(f.first, f.second.var());
          }
        }
      } else {
        if (with_static) {
          fields.emplace_back(f.first, f.second.var());
        }
      }
    }
    return fields;
  }

  const std::map<std::string_view, FieldInfo>& get_all_fields() const {
    return _fields;
  }

  Var var() {
    return _var;
  }

 private:
  std::map<std::string_view, FieldInfo> _fields;
  Var _var;
};

}  // namespace rr