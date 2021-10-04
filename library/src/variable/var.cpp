#include "er/variable/var.h"

#include "er/reflection/reflection.h"
#include "er/tools/format.h"
#include "er/types/all_types.h"

using namespace rr;

Var::Var() : _value(nullptr), _type(), _is_const(true) {
}

Var::Var(void* value, TypeId type, bool is_const) : _value(value), _type(type), _is_const(is_const) {
}

bool Var::operator==(const Var& other) const {
  return _type == other._type && _value == other._value;
}

bool Var::operator!=(const Var& other) const {
  return _type != other._type || _value != other._value;
}

void* Var::raw_mut() const {
  if (_is_const) {
    return nullptr;
  }

  return _value;
}

const void* Var::raw() const {
  return _value;
}

TypeId Var::type() const {
  return _type;
}

bool Var::is_const() const {
  return _is_const;
}

Error Var::error(TypeId type, TypeId desired_type) {
  return Error(format("Cannot cast {} to {}",       //
                      reflection::type_name(type),  //
                      reflection::type_name(desired_type)));
}
