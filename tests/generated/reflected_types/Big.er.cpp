#include "Big.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Big> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Big*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    map.insert({
      {"int1", FieldInfo(&p->int1, Access::kPublic)},
      {"int2", FieldInfo(&p->int2, Access::kPublic)},
      {"int3", FieldInfo(&p->int3, Access::kPublic)},
      {"int4", FieldInfo(&p->int4, Access::kPublic)},
      {"str1", FieldInfo(&p->str1, Access::kPublic)},
      {"str2", FieldInfo(&p->str2, Access::kPublic)},
      {"str3", FieldInfo(&p->str3, Access::kPublic)},
      {"str4", FieldInfo(&p->str4, Access::kPublic)},
      
    });

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(Big* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Big>::reflect,        //
                                                 &CommonActions<Big>::type_name,    //
                                                 &CommonActions<Big>::type_size,    //
                                                 &CommonActions<Big>::call_new,     //
                                                 &CommonActions<Big>::call_delete,  //
                                                 &CommonActions<Big>::copy)));
  return id;
}

}  // namespace er

// clang-format on
