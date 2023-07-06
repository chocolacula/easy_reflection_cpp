#pragma once

#include "common_actions.h"
#include "er/reflection/the_great_table.h"

namespace er {

struct BoolActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Bool(static_cast<bool*>(value), is_const));
  }

  static void call_delete(void* pointer, bool in_place) {
    if (!in_place) {
      delete static_cast<bool*>(pointer);
    }
  }
};

template <>
inline TypeId TypeId::get(bool* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&BoolActions::reflect,            //
                                                 &CommonActions<bool>::type_name,  //
                                                 &CommonActions<bool>::type_size,  //
                                                 &CommonActions<bool>::call_new,   //
                                                 &BoolActions::call_delete,        //
                                                 &CommonActions<bool>::copy,       //
                                                 &CommonActions<bool>::move)));
  return id;
}

}  // namespace er
