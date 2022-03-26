#include "bicycle.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Bicycle> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Bicycle*>(value);

    static std::map<std::string_view, FieldDesc> map {
      {"kIsCool", FieldDesc::create_static(&Bicycle::kIsCool, FieldAttributes::kPublic)},
      
      {"id", FieldDesc::create_member(p, &p->id, FieldAttributes::kPublic)},
      {"is_hardtail", FieldDesc::create_member(p, &p->is_hardtail, FieldAttributes::kPublic)},
      {"manufacturer", FieldDesc::create_member(p, &p->manufacturer, FieldAttributes::kPublic)},
      {"model", FieldDesc::create_member(p, &p->model, FieldAttributes::kPublic)},
      {"weight", FieldDesc::create_member(p, &p->frame_weight, FieldAttributes::kPublic)},
      {"wheel_size", FieldDesc::create_member(p, &p->wheel_size_inch, FieldAttributes::kPublic)},
      {"colors", FieldDesc::create_member(p, &p->colors, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(Bicycle* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Bicycle>::reflect,        //
                                                 &CommonActions<Bicycle>::type_name,    //
                                                 &CommonActions<Bicycle>::type_size,    //
                                                 &CommonActions<Bicycle>::call_new,     //
                                                 &CommonActions<Bicycle>::call_delete,  //
                                                 &CommonActions<Bicycle>::copy)));
  return id;
}

}  // namespace er

// clang-format on
