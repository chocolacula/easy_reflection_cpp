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

  static void construct(void* p) {
    if constexpr (!std::is_fundamental_v<T>) {
      new (p) T[size_v];
    }
    // do nothing for fundamental
  }

  static void destroy(void* p) {
    if constexpr (!std::is_fundamental_v<T>) {
      for (auto i = 0; i < size_v; i++) {
        static_cast<T*>(p)[i].~T();
      }
    }
    // do nothing for fundamental
  }

  static void copy(void* to, const void* from) {
    if constexpr (std::is_fundamental_v<T>) {
      std::memcpy(to, from, size_v);
    } else {
      for (auto i = 0; i < size_v; i++) {
        static_cast<T*>(to)[i] = static_cast<T*>(from)[i];
      }
    }
  }

  static void move(void* to, void* from) {
    if constexpr (std::is_fundamental_v<T>) {
      // copy fundamental types anyway
      std::memcpy(to, from, size_v);
    } else {
      for (auto i = 0; i < size_v; i++) {
        static_cast<T*>(to)[i] = std::move(static_cast<T*>(from)[i]);
      }
    }
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(T (*)[size_v]) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<T[size_v]>::reflect,      //
                                                 &CommonActions<T[size_v]>::type_name,  //
                                                 &TypeActions<T[size_v]>::type_size,    //
                                                 &TypeActions<T[size_v]>::construct,    //
                                                 &TypeActions<T[size_v]>::destroy,      //
                                                 &TypeActions<T[size_v]>::copy,         //
                                                 &TypeActions<T[size_v]>::move)));
  return id;
}

}  // namespace er
