#pragma once

#include "access.h"
#include "er/type_id.h"
#include "er/variable/var.h"

namespace er {

struct FieldDesc {

  static FieldDesc create_static(Var field, Access acc) {
    return FieldDesc(reinterpret_cast<uintptr_t>(field.raw()), field.type(), field.is_const(), true, acc);
  }

  static FieldDesc create_member(const void* base, Var member, Access acc) {
    return FieldDesc(delta(base, member.raw()), member.type(), member.is_const(), false, acc);
  }

  FieldDesc(uintptr_t value, TypeId type, bool is_const, bool is_static, Access acc)
      : _value(value), _type(type), _is_const(is_const), _is_static(is_static), _access(acc) {
  }

  uintptr_t value() const {
    return _value;
  }

  TypeId type() const {
    return _type;
  }

  bool is_const() const {
    return _is_const;
  }

  bool is_static() const {
    return _is_static;
  }

  Access access() const {
    return _access;
  }

 private:
  uintptr_t _value;  // pointer for static fields, offset for members

  TypeId _type;

  const bool _is_const;
  const bool _is_static;

  // it means
  // const bool _is_immutable;

  Access _access;

  static uintptr_t delta(const void* base, const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(base);
  }
};

}  // namespace er