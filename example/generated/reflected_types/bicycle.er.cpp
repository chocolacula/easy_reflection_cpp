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
      {"kIsCool", FieldDesc::create_static(Var(&Bicycle::kIsCool), Access::kPublic)},
      
      {"id", FieldDesc::create_member(value, Var(&p->id), Access::kPublic)},
      {"is_hardtail", FieldDesc::create_member(value, Var(&p->is_hardtail), Access::kPublic)},
      {"manufacturer", FieldDesc::create_member(value, Var(&p->manufacturer), Access::kPublic)},
      {"model", FieldDesc::create_member(value, Var(&p->model), Access::kPublic)},
      {"weight", FieldDesc::create_member(value, Var(&p->frame_weight), Access::kPublic)},
      {"wheel_size", FieldDesc::create_member(value, Var(&p->wheel_size_inch), Access::kPublic)},
      {"colors", FieldDesc::create_member(value, Var(&p->colors), Access::kPublic)},
      
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
