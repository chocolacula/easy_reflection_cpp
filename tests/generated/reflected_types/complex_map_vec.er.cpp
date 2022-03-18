#include "complex_map_vec.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<ComplexMapVec> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<ComplexMapVec*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"content", FieldDesc::create_member(value, Var(&p->content), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(ComplexMapVec* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<ComplexMapVec>::reflect,        //
                                                 &CommonActions<ComplexMapVec>::type_name,    //
                                                 &CommonActions<ComplexMapVec>::type_size,    //
                                                 &CommonActions<ComplexMapVec>::call_new,     //
                                                 &CommonActions<ComplexMapVec>::call_delete,  //
                                                 &CommonActions<ComplexMapVec>::copy)));
  return id;
}

}  // namespace er

// clang-format on
