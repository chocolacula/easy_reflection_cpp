#pragma once

#include "common_actions.h"
#include "er/reflection/the_great_table.h"

namespace er {

struct BoolActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return {Bool(static_cast<bool*>(value), is_const)};
  }
};

template <>
inline TypeId TypeId::get(bool* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&BoolActions::reflect,            //
                                                 &CommonActions<bool>::type_name,  //
                                                 &CommonActions<bool>::type_size,  //
                                                 &CommonActions<bool>::nop,        //
                                                 &CommonActions<bool>::nop,        //
                                                 &CommonActions<bool>::copy,       //
                                                 &CommonActions<bool>::move)));
  return id;
}

}  // namespace er
