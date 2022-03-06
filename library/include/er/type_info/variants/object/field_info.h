#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "access.h"
#include "er/type_id.h"
#include "field_desc.h"

namespace er {

class FieldInfo {
 public:
  FieldInfo(const void* base, const FieldDesc* data) : _base(base), _data(data) {
  }

  FieldInfo(const FieldInfo& other) {
    if (this == &other) {
      return;
    }
    _base = other._base;
    _data = other._data;
  }

  FieldInfo& operator=(const FieldInfo& other) {
    if (this == &other) {
      return *this;
    }
    _base = other._base;
    _data = other._data;
    return *this;
  }

  Var var() const {
    if (is_static()) {
      return {reinterpret_cast<void*>(_data->value()), _data->type(), _data->is_const()};
    }
    return {shift(_base, _data->value()), _data->type(), _data->is_const()};
  }

  bool is_const() const {
    return _data->is_const();
  }

  bool is_static() const {
    return _data->is_static();
  }

  Access access() const {
    return _data->access();
  }

  bool is_public() const {
    return (_data->access() & Access::kPublic) != Access::kNone;
  }

  bool is_protected() const {
    return (_data->access() & Access::kProtected) != Access::kNone;
  }

  bool is_private() const {
    return (_data->access() & Access::kPrivate) != Access::kNone;
  }

 private:
  const void* _base;
  const FieldDesc* _data;

  static void* shift(const void* base, size_t offset) {
    return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(base) + offset);
  }
};

}  // namespace er