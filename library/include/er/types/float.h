#pragma once

#include "common_actions.h"
#include "er/reflection/the_great_table.h"

namespace er {

template <typename T>
struct FloatActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Floating(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&FloatActions<T>::reflect,     //
                                                 &CommonActions<T>::type_name,  //
                                                 &CommonActions<T>::type_size,  //
                                                 &CommonActions<T>::nop,        //
                                                 &CommonActions<T>::nop,        //
                                                 &CommonActions<T>::copy,       //
                                                 &CommonActions<T>::move)));
  return id;
}

}  // namespace er
