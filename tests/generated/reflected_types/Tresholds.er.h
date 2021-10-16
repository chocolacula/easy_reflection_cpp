#pragma once

//clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/tresholds.h"

namespace rr {

template <>
struct TypeActions<Tresholds> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Tresholds*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    if (p != nullptr) {
      map.insert({
        {"u8_max", FieldInfo(&p->u8_max, Access::kPublic)},
        {"u16_max", FieldInfo(&p->u16_max, Access::kPublic)},
        {"u32_max", FieldInfo(&p->u32_max, Access::kPublic)},
        {"u64_max", FieldInfo(&p->u64_max, Access::kPublic)},
        {"i8_min", FieldInfo(&p->i8_min, Access::kPublic)},
        {"i8_max", FieldInfo(&p->i8_max, Access::kPublic)},
        {"i16_min", FieldInfo(&p->i16_min, Access::kPublic)},
        {"i16_max", FieldInfo(&p->i16_max, Access::kPublic)},
        {"i32_min", FieldInfo(&p->i32_min, Access::kPublic)},
        {"i32_max", FieldInfo(&p->i32_max, Access::kPublic)},
        {"i64_min", FieldInfo(&p->i64_min, Access::kPublic)},
        {"i64_max", FieldInfo(&p->i64_max, Access::kPublic)},
        {"f32_min", FieldInfo(&p->f32_min, Access::kPublic)},
        {"f32_max", FieldInfo(&p->f32_max, Access::kPublic)},
        {"f64_min", FieldInfo(&p->f64_min, Access::kPublic)},
        {"f64_max", FieldInfo(&p->f64_max, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(Tresholds* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Tresholds>::reflect,        //
                                                 &CommonActions<Tresholds>::type_name,    //
                                                 &CommonActions<Tresholds>::type_size,    //
                                                 &CommonActions<Tresholds>::call_new,     //
                                                 &CommonActions<Tresholds>::call_delete,  //
                                                 &CommonActions<Tresholds>::copy)));
  return id;
}

}  // namespace rr

//clang-format on
