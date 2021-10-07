#pragma once

#include <cstdint>
#include <type_traits>

#include "access.h"
#include "er/variable/var.h"

namespace rr {

class FieldInfo {
 public:
  template <typename T>
  constexpr FieldInfo(const T* ptr, Access acc) : _var(ptr), _access(acc) {
  }

  template <typename T>
  constexpr FieldInfo(T* ptr, Access acc) : _var(ptr), _access(acc) {
  }

  Var var() const {
    return _var;
  }

  Access access() const {
    return _access;
  }

  bool is_public() const {
    return (_access & Access::kPublic) != Access::kNone;
  }

  bool is_protected() const {
    return (_access & Access::kProtected) != Access::kNone;
  }

  bool is_private() const {
    return (_access & Access::kPrivate) != Access::kNone;
  }

  bool is_static() const {
    return (_access & Access::kStatic) != Access::kNone;
  }

 private:
  Var _var;
  Access _access;
};

}  // namespace rr