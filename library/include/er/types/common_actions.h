#pragma once

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

  static void* call_new(void* place, size_t place_size) {
    if (place_size >= sizeof(T)) {
      new (place) T();
      return place;
    }
    auto* p = new T();
    return p;
  }

  static void call_delete(void* pointer, bool in_place) {
    if (in_place) {
      static_cast<T*>(pointer)->~T();
    } else {
      delete static_cast<T*>(pointer);
    }
  }

  static void copy(void* to, const void* from) {
    *static_cast<T*>(to) = *static_cast<const T*>(from);
  }
};

}  // namespace er