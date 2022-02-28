#include "various_complex_value.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Various::ComplexValue> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Various::ComplexValue*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    if (p != nullptr) {
      map.insert({
        {"v1", FieldInfo(&p->v1, Access::kPublic)},
        {"v2", FieldInfo(&p->v2, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(Various::ComplexValue* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Various::ComplexValue>::reflect,        //
                                                 &CommonActions<Various::ComplexValue>::type_name,    //
                                                 &CommonActions<Various::ComplexValue>::type_size,    //
                                                 &CommonActions<Various::ComplexValue>::call_new,     //
                                                 &CommonActions<Various::ComplexValue>::call_delete,  //
                                                 &CommonActions<Various::ComplexValue>::copy)));
  return id;
}

}  // namespace er

// clang-format on
