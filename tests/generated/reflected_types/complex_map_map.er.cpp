#include "complex_map_map.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<ComplexMapMap> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<ComplexMapMap*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"content", FieldDesc::create_member(value, Var(&p->content), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(ComplexMapMap* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<ComplexMapMap>::reflect,        //
                                                 &CommonActions<ComplexMapMap>::type_name,    //
                                                 &CommonActions<ComplexMapMap>::type_size,    //
                                                 &CommonActions<ComplexMapMap>::call_new,     //
                                                 &CommonActions<ComplexMapMap>::call_delete,  //
                                                 &CommonActions<ComplexMapMap>::copy)));
  return id;
}

}  // namespace er

// clang-format on
