#include "complex_map_obj.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<ComplexMapObj> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<ComplexMapObj*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"content", FieldDesc::create_member(value, Var(&p->content), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(ComplexMapObj* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<ComplexMapObj>::reflect,        //
                                                 &CommonActions<ComplexMapObj>::type_name,    //
                                                 &CommonActions<ComplexMapObj>::type_size,    //
                                                 &CommonActions<ComplexMapObj>::call_new,     //
                                                 &CommonActions<ComplexMapObj>::call_delete,  //
                                                 &CommonActions<ComplexMapObj>::copy)));
  return id;
}

}  // namespace er

// clang-format on
