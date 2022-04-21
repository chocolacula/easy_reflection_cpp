#include "big_one.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<BigOne> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<BigOne*>(value);

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
TypeId TypeId::get(BigOne* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<BigOne>::reflect,        //
                                                 &CommonActions<BigOne>::type_name,    //
                                                 &CommonActions<BigOne>::type_size,    //
                                                 &CommonActions<BigOne>::call_new,     //
                                                 &CommonActions<BigOne>::call_delete,  //
                                                 &CommonActions<BigOne>::copy)));
  return id;
}

}  // namespace er

// clang-format on
