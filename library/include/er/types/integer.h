#pragma once

#include "common_actions.h"
#include "er/reflection/the_great_table.h"

namespace er {

template <typename T>
struct IntActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Integer(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<std::is_integral_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&IntActions<T>::reflect,       //
                                                 &CommonActions<T>::type_name,  //
                                                 &CommonActions<T>::type_size,  //
                                                 &CommonActions<T>::construct,  //
                                                 &CommonActions<T>::destroy,    //
                                                 &CommonActions<T>::copy,       //
                                                 &CommonActions<T>::move)));
  return id;
}

}  // namespace er
