#pragma once

#include "common_actions.h"
#include "er/reflection/the_great_table.h"

namespace er {

template <typename T>
struct StringActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(String(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<is_string_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&StringActions<T>::reflect,      //
                                                 &CommonActions<T>::type_name,    //
                                                 &CommonActions<T>::type_size,    //
                                                 &CommonActions<T>::call_new,     //
                                                 &CommonActions<T>::call_delete,  //
                                                 &CommonActions<T>::copy,         //
                                                 &CommonActions<T>::move)));
  return id;
}

}  // namespace er
