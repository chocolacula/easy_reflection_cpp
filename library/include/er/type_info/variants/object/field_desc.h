#pragma once

#include <cstdint>
#include <type_traits>

#include "access.h"
#include "er/tools/traits.h"
#include "er/type_id.h"
#include "er/variable/var.h"

namespace er {

struct FieldDesc {

  template <typename T, typename ClassT>
  FieldDesc(T ClassT::*ptr, Access acc)
      : _value(static_cast<uintptr_t>(delta(ptr))),
        _type(TypeId::get<std::remove_const_t<T>>()),
        _acc(acc),
        _is_readonly(is_ref_type_v<T>) {
  }

  template <typename T>
  FieldDesc(T* ptr, Access acc)
      : _value(reinterpret_cast<uintptr_t>(ptr)),
        _type(TypeId::get<std::remove_const_t<T>>()),
        _acc(acc),
        _is_readonly(is_ref_type_v<T>) {
  }

  uintptr_t value() const {
    return _value;
  }

  TypeId type() const {
    return _type;
  }

  bool is_const() const {
    return (_acc & Access::kConst) != Access::kNone;
  }

  bool is_static() const {
    return (_acc & Access::kStatic) != Access::kNone;
  }

  bool is_public() const {
    return (_acc & Access::kPublic) != Access::kNone;
  }

  bool is_protected() const {
    return (_acc & Access::kProtected) != Access::kNone;
  }

  bool is_private() const {
    return (_acc & Access::kPrivate) != Access::kNone;
  }

  Access access() const {
    return _acc;
  }

  bool is_readonly() const {
    return _is_readonly;
  }

 private:
  const uintptr_t _value;  // pointer for static fields, offset for members
  const TypeId _type;

  const Access _acc;
  const bool _is_readonly;

  template <class ClassT, typename T>
  std::ptrdiff_t delta(T ClassT::*ptr) {
    // NULL not nullptr_t
    return reinterpret_cast<std::ptrdiff_t>(&(reinterpret_cast<ClassT const*>(NULL)->*ptr));
  }
};
}  // namespace er
