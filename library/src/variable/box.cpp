#include "er/variable/box.h"

#include <_types/_uint8_t.h>

#include "er/buff_alloc.h"
#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

using namespace er;

Box::Box(TypeId id, std::pmr::polymorphic_allocator<uint8_t>* alloc) : _alloc(alloc) {
  _var = Var(_alloc->allocate(reflection::type_size(id)), id, false);
  reflection::construct(_var);
}

Box::~Box() {
  auto size = reflection::type_size(_var.type());
  auto* p = reinterpret_cast<uint8_t*>(_var.raw_mut());

  reflection::destroy(_var);
  _alloc->deallocate(p, size);
}

Var Box::var() {
  return _var;
}

Box Box::clone() {
  Box new_one(_var.type());
  reflection::copy(new_one.var(), var());
  return new_one;
}

// TODO make it available only while testing
bool Box::uses_heap() const {
  return true;
}
