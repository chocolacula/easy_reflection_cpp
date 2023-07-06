#pragma once

#include <cstddef>
#include <cstring>

#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <typename T, size_t size_v>
struct TypeActions<T[size_v]> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Array(static_cast<T(*)[size_v]>(value), is_const));
  }

  static size_t type_size() {
    return sizeof(T[size_v]);
  }

  static void* call_new(void* place, size_t place_size) {
    if (place_size >= sizeof(T)) {
      new (place) T[size_v]{};
      return place;
    }
    auto* p = new T[size_v];
    return p;
  }

  static void call_delete(void* pointer, bool in_place) {
    if (!in_place) {
      delete[] static_cast<T*>(pointer);
    }
  }

  static void copy(void* to, const void* from) {
    for (auto i = 0; i < size_v; i++) {
      static_cast<T*>(to)[i] = static_cast<T*>(from)[i];
    }
  }

  static void move(void* to, void* from) {
    for (auto i = 0; i < size_v; i++) {
      static_cast<T*>(to)[i] = std::move(static_cast<T*>(from)[i]);
    }
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(T (*)[size_v]) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<T[size_v]>::reflect,      //
                                                 &CommonActions<T[size_v]>::type_name,  //
                                                 &TypeActions<T[size_v]>::type_size,    //
                                                 &TypeActions<T[size_v]>::call_new,     //
                                                 &TypeActions<T[size_v]>::call_delete,  //
                                                 &TypeActions<T[size_v]>::copy,         //
                                                 &TypeActions<T[size_v]>::move)));
  return id;
}

}  // namespace er
