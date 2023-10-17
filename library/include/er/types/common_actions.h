#pragma once

#include <_types/_uint8_t.h>

#include <cstddef>

#include "er/reflection/the_great_table.h"
#include "er/tools/names.h"

namespace er {

template <typename T>
struct CommonActions {
  static std::string_view type_name() {
    static auto name = Names<T>::get();
    return name;
  }

  static size_t type_size() {
    return sizeof(T);
  }

  static uint8_t* call_new(Actions::palloc_t* alloc, size_t n) {
    return alloc->allocate(n);
  }

  static void call_delete(Actions::palloc_t* alloc, uint8_t* p, size_t n) {
    return alloc->deallocate(p, n);
  }

  static void copy(void* to, const void* from) {
    *static_cast<T*>(to) = *static_cast<const T*>(from);
  }

  static void move(void* to, void* from) {
    *static_cast<T*>(to) = std::move(*static_cast<T*>(from));
  }
};

}  // namespace er
