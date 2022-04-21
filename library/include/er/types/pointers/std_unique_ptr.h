#pragma once

#include <memory>

#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <typename T>
struct TypeActions<std::unique_ptr<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return {Pointer(static_cast<std::shared_ptr<T>*>(value), is_const)};
  }

  static void* call_new(void* place, size_t place_size) {
    if (place_size >= sizeof(std::unique_ptr<T>)) {
      new (place) std::unique_ptr<T>(new T());
      return place;
    }
    throw std::runtime_error("Not enough memory for new std::unique_ptr");
  }

  static void call_delete(void* pointer, bool in_place) {
    if (in_place) {
      static_cast<std::unique_ptr<T>*>(pointer)->~unique_ptr();
    } else {
      throw std::runtime_error("std::unique_ptr should be in_place");
    }
  }

  static void copy(void* to, const void* from) {
    // move instead of copy
    auto nc_ptr = static_cast<std::unique_ptr<T>*>(const_cast<void*>(from));
    *static_cast<std::unique_ptr<T>*>(to) = std::move(*nc_ptr);
  }
};

template <typename T>
TypeId TypeId::get(std::unique_ptr<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<std::unique_ptr<T>>::reflect,      //
                                                 &CommonActions<std::unique_ptr<T>>::type_name,  //
                                                 &CommonActions<std::unique_ptr<T>>::type_size,  //
                                                 &TypeActions<std::unique_ptr<T>>::call_new,     //
                                                 &TypeActions<std::unique_ptr<T>>::call_delete,  //
                                                 &TypeActions<std::unique_ptr<T>>::copy)));
  return id;
}

}  // namespace er