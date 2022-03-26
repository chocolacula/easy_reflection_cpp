#include "various.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Various> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Various*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"ch", FieldDesc::create_member(p, &p->ch, FieldAttributes::kPublic)},
      {"uch", FieldDesc::create_member(p, &p->uch, FieldAttributes::kPublic)},
      {"sch", FieldDesc::create_member(p, &p->sch, FieldAttributes::kPublic)},
      {"u8", FieldDesc::create_member(p, &p->u8, FieldAttributes::kPublic)},
      {"u16", FieldDesc::create_member(p, &p->u16, FieldAttributes::kPublic)},
      {"u32", FieldDesc::create_member(p, &p->u32, FieldAttributes::kPublic)},
      {"u64", FieldDesc::create_member(p, &p->u64, FieldAttributes::kPublic)},
      {"i8", FieldDesc::create_member(p, &p->i8, FieldAttributes::kPublic)},
      {"i16", FieldDesc::create_member(p, &p->i16, FieldAttributes::kPublic)},
      {"i32", FieldDesc::create_member(p, &p->i32, FieldAttributes::kPublic)},
      {"i64", FieldDesc::create_member(p, &p->i64, FieldAttributes::kPublic)},
      {"f32", FieldDesc::create_member(p, &p->f32, FieldAttributes::kPublic)},
      {"f64", FieldDesc::create_member(p, &p->f64, FieldAttributes::kPublic)},
      {"arr", FieldDesc::create_member(p, &p->arr, FieldAttributes::kPublic)},
      {"str", FieldDesc::create_member(p, &p->str, FieldAttributes::kPublic)},
      {"std_arr", FieldDesc::create_member(p, &p->std_arr, FieldAttributes::kPublic)},
      {"vec", FieldDesc::create_member(p, &p->vec, FieldAttributes::kPublic)},
      {"list", FieldDesc::create_member(p, &p->list, FieldAttributes::kPublic)},
      {"deq", FieldDesc::create_member(p, &p->deq, FieldAttributes::kPublic)},
      {"que", FieldDesc::create_member(p, &p->que, FieldAttributes::kPublic)},
      {"stack", FieldDesc::create_member(p, &p->stack, FieldAttributes::kPublic)},
      {"set", FieldDesc::create_member(p, &p->set, FieldAttributes::kPublic)},
      {"un_set", FieldDesc::create_member(p, &p->un_set, FieldAttributes::kPublic)},
      {"map", FieldDesc::create_member(p, &p->map, FieldAttributes::kPublic)},
      {"un_map", FieldDesc::create_member(p, &p->un_map, FieldAttributes::kPublic)},
      {"obj", FieldDesc::create_member(p, &p->obj, FieldAttributes::kPublic)},
      {"monstro", FieldDesc::create_member(p, &p->monstro, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(Various* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Various>::reflect,        //
                                                 &CommonActions<Various>::type_name,    //
                                                 &CommonActions<Various>::type_size,    //
                                                 &CommonActions<Various>::call_new,     //
                                                 &CommonActions<Various>::call_delete,  //
                                                 &CommonActions<Various>::copy)));
  return id;
}

}  // namespace er

// clang-format on
