#pragma once

#include "er/expected.h"
#include "er/type_id.h"

namespace rr {

/// The abstraction from type and const modifier
/// all types represented like a pointer + type id + const flag
/// the same representation for each type
struct Var {
  Var();
  Var(void* value, TypeId type, bool is_const);

  template <typename T>
  explicit Var(const T* value) : _value(const_cast<T*>(value)), _type(TypeId::get<T>()), _is_const(true) {
  }

  template <typename T>
  explicit Var(T* value, bool is_const = false) : _value(value), _type(TypeId::get(value)), _is_const(is_const) {
  }

  bool operator==(const Var& other) const;
  bool operator!=(const Var& other) const;

  void* raw_mut() const;
  const void* raw() const;

  TypeId type() const;

  bool is_const() const;

  /// runtime type check and cast
  template <typename T>
  Expected<T*> rt_cast() const {

    if (std::is_const_v<T> == false && _is_const) {
      return Error("The type under Var has const qualifier, cannot cast to mutable");
    }

    auto desired_type = TypeId::get<T>();

    if (desired_type != _type) {
      return error(_type, desired_type);
    }

    return static_cast<T*>(_value);
  }

 private:
  void* _value;
  TypeId _type;
  bool _is_const;

  static Error error(TypeId type, TypeId desired_type);
};

}  // namespace rr