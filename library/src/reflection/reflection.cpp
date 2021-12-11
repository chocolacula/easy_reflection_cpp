#include "er/reflection/reflection.h"

#include <cstddef>
#include <iostream>

#include "er/reflection/the_great_table.h"
#include "er/types/all_types.h"
#include "sprint.h"

using namespace er;

TypeInfo reflection::reflect(Var variable) {
  return TheGreatTable::data()[variable.type().number()].reflect(const_cast<void*>(variable.raw()),
                                                                 variable.is_const());
}

std::string reflection::sprint(const TypeInfo& info) {
  std::string result;
  sprint(info, &result, 0);
  return result;
}

std::string reflection::sprint(Var var) {
  return sprint(reflect(var));
}

void reflection::print(const TypeInfo& info) {
  std::cout << sprint(info) << std::flush;
}

void reflection::print(Var var) {
  print(reflect(var));
}

std::string_view reflection::type_name(TypeId id) {
  return TheGreatTable::data()[id.number()].type_name();
}

size_t reflection::type_size(TypeId id) {
  return TheGreatTable::data()[id.number()].type_size();
}

void* reflection::call_new(TypeId id, void* place, size_t place_size) {
  return TheGreatTable::data()[id.number()].call_new(place, place_size);
}

void reflection::call_delete(Var variable, bool in_place) {
  TheGreatTable::data()[variable.type().number()].call_delete(variable.raw_mut(), in_place);
  variable.dispose();
}

Expected<None> reflection::copy(Var to, Var from) {
  if (to.is_const()) {
    return Error("Cannot assign to const value");
  }
  if (to.type() != from.type()) {
    return Error(format("Cannot copy {} to {}", type_name(from.type()), type_name(to.type())));
  }
  TheGreatTable::data()[to.type().number()].copy(to.raw_mut(), from.raw());
  return None();
}
