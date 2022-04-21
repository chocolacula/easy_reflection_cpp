#pragma once

#include <memory>
#include <stdexcept>

#include "er/reflection/the_great_table.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/pointer/std_shared_ptr.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <typename T>
struct TypeActions<std::shared_ptr<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return {Pointer(static_cast<std::shared_ptr<T>*>(value), is_const)};
  }

  static void* call_new(void* place, size_t place_size) {
    if (place_size >= sizeof(std::shared_ptr<T>)) {
      new (place) std::shared_ptr<T>(new T());
      return place;
    }
    throw std::runtime_error("Not enough memory for new std::shared_ptr");
  }

  static void call_delete(void* pointer, bool in_place) {
    if (in_place) {
      static_cast<std::shared_ptr<T>*>(pointer)->~shared_ptr();
    } else {
      throw std::runtime_error("std::shared_ptr should be in_place");
    }
  }
};

template <typename T>
TypeId TypeId::get(std::shared_ptr<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<std::shared_ptr<T>>::reflect,      //
                                                 &CommonActions<std::shared_ptr<T>>::type_name,  //
                                                 &CommonActions<std::shared_ptr<T>>::type_size,  //
                                                 &TypeActions<std::shared_ptr<T>>::call_new,     //
                                                 &TypeActions<std::shared_ptr<T>>::call_delete,  //
                                                 &CommonActions<std::shared_ptr<T>>::copy)));
  return id;
}

}  // namespace er