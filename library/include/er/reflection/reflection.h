#pragma once

#include "er/type_info/type_info.h"
#include "type_name.h"

namespace er::reflection {

TypeInfo reflect(Var variable);

template <typename T>
TypeInfo reflect(T* pointer) {
  return reflection::reflect(Var(pointer));
}

template <typename T>
TypeInfo reflect(const T* pointer) {
  return reflection::reflect(Var(pointer));
}

std::string sprint(const TypeInfo& info);
std::string sprint(Var var);

template <typename T>
std::string sprint(const T* pointer) {
  return sprint(reflect(pointer));
}

void print(const TypeInfo& info);
void print(Var var);

template <typename T>
void print(const T* pointer) {
  print(reflect(pointer));
}

size_t type_size(TypeId id);

void* call_new(TypeId id, std::pmr::polymorphic_allocator<uint8_t>* alloc, size_t n);
void call_delete(Var variable, std::pmr::polymorphic_allocator<uint8_t>* alloc, size_t n);

Expected<None> copy(Var to, Var from);

}  // namespace er::reflection
