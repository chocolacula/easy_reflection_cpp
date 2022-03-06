#include "tresholds.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Tresholds> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Tresholds*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"u8_max", FieldDesc::create_member(value, Var(&p->u8_max), Access::kPublic)},
      {"u16_max", FieldDesc::create_member(value, Var(&p->u16_max), Access::kPublic)},
      {"u32_max", FieldDesc::create_member(value, Var(&p->u32_max), Access::kPublic)},
      {"u64_max", FieldDesc::create_member(value, Var(&p->u64_max), Access::kPublic)},
      {"i8_min", FieldDesc::create_member(value, Var(&p->i8_min), Access::kPublic)},
      {"i8_max", FieldDesc::create_member(value, Var(&p->i8_max), Access::kPublic)},
      {"i16_min", FieldDesc::create_member(value, Var(&p->i16_min), Access::kPublic)},
      {"i16_max", FieldDesc::create_member(value, Var(&p->i16_max), Access::kPublic)},
      {"i32_min", FieldDesc::create_member(value, Var(&p->i32_min), Access::kPublic)},
      {"i32_max", FieldDesc::create_member(value, Var(&p->i32_max), Access::kPublic)},
      {"i64_min", FieldDesc::create_member(value, Var(&p->i64_min), Access::kPublic)},
      {"i64_max", FieldDesc::create_member(value, Var(&p->i64_max), Access::kPublic)},
      {"f32_min", FieldDesc::create_member(value, Var(&p->f32_min), Access::kPublic)},
      {"f32_max", FieldDesc::create_member(value, Var(&p->f32_max), Access::kPublic)},
      {"f64_min", FieldDesc::create_member(value, Var(&p->f64_min), Access::kPublic)},
      {"f64_max", FieldDesc::create_member(value, Var(&p->f64_max), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
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

}  // namespace er

// clang-format on
