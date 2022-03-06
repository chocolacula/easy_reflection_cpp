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
      
      {"ch", FieldDesc::create_member(value, Var(&p->ch), Access::kPublic)},
      {"uch", FieldDesc::create_member(value, Var(&p->uch), Access::kPublic)},
      {"sch", FieldDesc::create_member(value, Var(&p->sch), Access::kPublic)},
      {"u8", FieldDesc::create_member(value, Var(&p->u8), Access::kPublic)},
      {"u16", FieldDesc::create_member(value, Var(&p->u16), Access::kPublic)},
      {"u32", FieldDesc::create_member(value, Var(&p->u32), Access::kPublic)},
      {"u64", FieldDesc::create_member(value, Var(&p->u64), Access::kPublic)},
      {"i8", FieldDesc::create_member(value, Var(&p->i8), Access::kPublic)},
      {"i16", FieldDesc::create_member(value, Var(&p->i16), Access::kPublic)},
      {"i32", FieldDesc::create_member(value, Var(&p->i32), Access::kPublic)},
      {"i64", FieldDesc::create_member(value, Var(&p->i64), Access::kPublic)},
      {"f32", FieldDesc::create_member(value, Var(&p->f32), Access::kPublic)},
      {"f64", FieldDesc::create_member(value, Var(&p->f64), Access::kPublic)},
      {"arr", FieldDesc::create_member(value, Var(&p->arr), Access::kPublic)},
      {"str", FieldDesc::create_member(value, Var(&p->str), Access::kPublic)},
      {"std_arr", FieldDesc::create_member(value, Var(&p->std_arr), Access::kPublic)},
      {"vec", FieldDesc::create_member(value, Var(&p->vec), Access::kPublic)},
      {"list", FieldDesc::create_member(value, Var(&p->list), Access::kPublic)},
      {"deq", FieldDesc::create_member(value, Var(&p->deq), Access::kPublic)},
      {"que", FieldDesc::create_member(value, Var(&p->que), Access::kPublic)},
      {"stack", FieldDesc::create_member(value, Var(&p->stack), Access::kPublic)},
      {"set", FieldDesc::create_member(value, Var(&p->set), Access::kPublic)},
      {"un_set", FieldDesc::create_member(value, Var(&p->un_set), Access::kPublic)},
      {"map", FieldDesc::create_member(value, Var(&p->map), Access::kPublic)},
      {"un_map", FieldDesc::create_member(value, Var(&p->un_map), Access::kPublic)},
      {"obj", FieldDesc::create_member(value, Var(&p->obj), Access::kPublic)},
      {"monstro", FieldDesc::create_member(value, Var(&p->monstro), Access::kPublic)},
      
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
