#include "er/variable/box.h"

#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

using namespace rr;

Box::Box(TypeId id) : _type(id) {
  auto* ptr = reflection::call_new(id, &_data.stack_mem[0], kMemSize);
  _optimized = (ptr == &_data.stack_mem[0]);
}

Box::~Box() {
  reflection::call_delete(Var(&_data.stack_mem[0], _type, false), _optimized);
}

Var Box::var() {
  if (_optimized) {
    return Var(&_data.stack_mem[0], _type, false);
  }
  return Var(_data.ptr, _type, false);
}

Box Box::clone() {
  Box new_one(_type);
  reflection::copy(new_one.var(), var());
  return new_one;
}
