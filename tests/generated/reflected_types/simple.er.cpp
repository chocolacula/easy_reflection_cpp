#include "simple.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Simple> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Simple*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"v1", FieldDesc::create_member(p, &p->v1, FieldAttributes::kPublic)},
      {"v2", FieldDesc::create_member(p, &p->v2, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(Simple* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Simple>::reflect,        //
                                                 &CommonActions<Simple>::type_name,    //
                                                 &CommonActions<Simple>::type_size,    //
                                                 &CommonActions<Simple>::call_new,     //
                                                 &CommonActions<Simple>::call_delete,  //
                                                 &CommonActions<Simple>::copy)));
  return id;
}

}  // namespace er

// clang-format on
