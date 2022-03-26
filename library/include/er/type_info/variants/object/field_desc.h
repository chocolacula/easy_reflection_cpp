#pragma once

#include <type_traits>

#include "er/tools/traits.h"
#include "er/type_id.h"
#include "er/variable/var.h"
#include "field_attributes.h"

namespace er {

struct FieldDesc {

  template <typename T>
  static FieldDesc create_static(T* ptr, FieldAttributes atr) {
    atr = atr | FieldAttributes::kStatic;
    if (is_ref_type_v<T>) {
      atr = atr | FieldAttributes::kReadOnly;
    }
    return {reinterpret_cast<uintptr_t>(ptr),       //
            TypeId::get<std::remove_const_t<T>>(),  //
            std::is_const_v<T>,                     //
            atr};
  }

  template <typename T>
  static FieldDesc create_member(void* base, T* ptr, FieldAttributes atr) {
    if (is_ref_type_v<T>) {
      atr = atr | FieldAttributes::kReadOnly;
    }
    return {delta(base, ptr),                       //
            TypeId::get<std::remove_const_t<T>>(),  //
            std::is_const_v<T>,                     //
            atr};
  }

  FieldDesc(uintptr_t value, TypeId type, bool is_const, FieldAttributes acc)
      : _value(value), _type(type), _is_const(is_const), _atr(acc) {
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
    return (_atr & FieldAttributes::kStatic) != FieldAttributes::kNone;
  }

  bool is_public() const {
    return (_atr & FieldAttributes::kPublic) != FieldAttributes::kNone;
  }

  bool is_protected() const {
    return (_atr & FieldAttributes::kProtected) != FieldAttributes::kNone;
  }

  bool is_private() const {
    return (_atr & FieldAttributes::kPrivate) != FieldAttributes::kNone;
  }

  FieldAttributes attributes() const {
    return _atr;
  }

 private:
  uintptr_t _value;  // pointer for static fields, offset for members
  TypeId _type;
  const bool _is_const;

  FieldAttributes _atr;

  static uintptr_t delta(const void* base, const void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(base);
  }
};

}  // namespace er