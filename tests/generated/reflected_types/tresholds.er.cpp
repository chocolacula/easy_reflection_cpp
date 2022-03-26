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
      
      {"u8_max", FieldDesc::create_member(p, &p->u8_max, FieldAttributes::kPublic)},
      {"u16_max", FieldDesc::create_member(p, &p->u16_max, FieldAttributes::kPublic)},
      {"u32_max", FieldDesc::create_member(p, &p->u32_max, FieldAttributes::kPublic)},
      {"u64_max", FieldDesc::create_member(p, &p->u64_max, FieldAttributes::kPublic)},
      {"i8_min", FieldDesc::create_member(p, &p->i8_min, FieldAttributes::kPublic)},
      {"i8_max", FieldDesc::create_member(p, &p->i8_max, FieldAttributes::kPublic)},
      {"i16_min", FieldDesc::create_member(p, &p->i16_min, FieldAttributes::kPublic)},
      {"i16_max", FieldDesc::create_member(p, &p->i16_max, FieldAttributes::kPublic)},
      {"i32_min", FieldDesc::create_member(p, &p->i32_min, FieldAttributes::kPublic)},
      {"i32_max", FieldDesc::create_member(p, &p->i32_max, FieldAttributes::kPublic)},
      {"i64_min", FieldDesc::create_member(p, &p->i64_min, FieldAttributes::kPublic)},
      {"i64_max", FieldDesc::create_member(p, &p->i64_max, FieldAttributes::kPublic)},
      {"f32_min", FieldDesc::create_member(p, &p->f32_min, FieldAttributes::kPublic)},
      {"f32_max", FieldDesc::create_member(p, &p->f32_max, FieldAttributes::kPublic)},
      {"f64_min", FieldDesc::create_member(p, &p->f64_min, FieldAttributes::kPublic)},
      {"f64_max", FieldDesc::create_member(p, &p->f64_max, FieldAttributes::kPublic)},
      {"inf", FieldDesc::create_member(p, &p->inf, FieldAttributes::kPublic)},
      {"nan", FieldDesc::create_member(p, &p->nan, FieldAttributes::kPublic)},
      
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
