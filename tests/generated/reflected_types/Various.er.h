#pragma once

//clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/various.h"

namespace er {

template <>
struct TypeActions<Various> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Various*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    if (p != nullptr) {
      map.insert({
        {"ch", FieldInfo(&p->ch, Access::kPublic)},
        {"uch", FieldInfo(&p->uch, Access::kPublic)},
        {"sch", FieldInfo(&p->sch, Access::kPublic)},
        {"u8", FieldInfo(&p->u8, Access::kPublic)},
        {"u16", FieldInfo(&p->u16, Access::kPublic)},
        {"u32", FieldInfo(&p->u32, Access::kPublic)},
        {"u64", FieldInfo(&p->u64, Access::kPublic)},
        {"i8", FieldInfo(&p->i8, Access::kPublic)},
        {"i16", FieldInfo(&p->i16, Access::kPublic)},
        {"i32", FieldInfo(&p->i32, Access::kPublic)},
        {"i64", FieldInfo(&p->i64, Access::kPublic)},
        {"f32", FieldInfo(&p->f32, Access::kPublic)},
        {"f64", FieldInfo(&p->f64, Access::kPublic)},
        {"arr", FieldInfo(&p->arr, Access::kPublic)},
        {"str", FieldInfo(&p->str, Access::kPublic)},
        {"std_arr", FieldInfo(&p->std_arr, Access::kPublic)},
        {"vec", FieldInfo(&p->vec, Access::kPublic)},
        {"list", FieldInfo(&p->list, Access::kPublic)},
        {"deq", FieldInfo(&p->deq, Access::kPublic)},
        {"que", FieldInfo(&p->que, Access::kPublic)},
        {"stack", FieldInfo(&p->stack, Access::kPublic)},
        {"set", FieldInfo(&p->set, Access::kPublic)},
        {"un_set", FieldInfo(&p->un_set, Access::kPublic)},
        {"map", FieldInfo(&p->map, Access::kPublic)},
        {"un_map", FieldInfo(&p->un_map, Access::kPublic)},
        {"obj", FieldInfo(&p->obj, Access::kPublic)},
        {"monstro", FieldInfo(&p->monstro, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
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

//clang-format on
