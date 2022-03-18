#include "er/variable/box.h"

#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

using namespace er;

Box::Box(TypeId id) : _type(id) {
  auto* ptr = reflection::call_new(id, &_data.stack_mem[0], kMemSize);

  if (ptr == &_data.stack_mem[0]) {
    _optimized = true;
  } else {
    _optimized = false;
    _data.ptr = ptr;
  }
}

Box::~Box() {
  void* ptr = _optimized ? &_data.stack_mem[0] : _data.ptr;
  reflection::call_delete(Var(ptr, _type, false), _optimized);
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

// TODO make it available only while testing
bool Box::uses_heap() const {
  return !_optimized;
}
