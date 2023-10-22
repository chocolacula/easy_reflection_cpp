#pragma once

#include <memory>

#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <typename T>
struct TypeActions<std::unique_ptr<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return {Pointer(static_cast<std::unique_ptr<T>*>(value), is_const)};
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
                                                 &CommonActions<std::unique_ptr<T>>::construct,  //
                                                 &CommonActions<std::unique_ptr<T>>::destroy,    //
                                                 &TypeActions<std::unique_ptr<T>>::copy,         //
                                                 &CommonActions<std::unique_ptr<T>>::move)));
  return id;
}

}  // namespace er
