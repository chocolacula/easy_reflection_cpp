#include "big.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Big> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Big*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"int1", FieldDesc::create_member(p, &p->int1, FieldAttributes::kPublic)},
      {"int2", FieldDesc::create_member(p, &p->int2, FieldAttributes::kPublic)},
      {"int3", FieldDesc::create_member(p, &p->int3, FieldAttributes::kPublic)},
      {"int4", FieldDesc::create_member(p, &p->int4, FieldAttributes::kPublic)},
      {"str1", FieldDesc::create_member(p, &p->str1, FieldAttributes::kPublic)},
      {"str2", FieldDesc::create_member(p, &p->str2, FieldAttributes::kPublic)},
      {"str3", FieldDesc::create_member(p, &p->str3, FieldAttributes::kPublic)},
      {"str4", FieldDesc::create_member(p, &p->str4, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
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
