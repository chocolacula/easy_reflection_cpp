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
};

template <typename T>
TypeId TypeId::get(std::shared_ptr<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<std::shared_ptr<T>>::reflect,      //
                                                 &CommonActions<std::shared_ptr<T>>::type_name,  //
                                                 &CommonActions<std::shared_ptr<T>>::type_size,  //
                                                 &CommonActions<std::shared_ptr<T>>::construct,  //
                                                 &CommonActions<std::shared_ptr<T>>::destroy,    //
                                                 &CommonActions<std::shared_ptr<T>>::copy,       //
                                                 &CommonActions<std::shared_ptr<T>>::move)));
  return id;
}

}  // namespace er
