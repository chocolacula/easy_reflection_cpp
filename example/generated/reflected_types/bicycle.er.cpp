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

    std::map<std::string_view, FieldInfo> map {
      {"kIsCool", FieldInfo(&Bicycle::kIsCool, Access::kPublic | Access::kStatic)},
      
    };

    if (p != nullptr) {
      map.insert({
        {"id", FieldInfo(&p->id, Access::kPublic)},
        {"is_hardtail", FieldInfo(&p->is_hardtail, Access::kPublic)},
        {"manufacturer", FieldInfo(&p->manufacturer, Access::kPublic)},
        {"model", FieldInfo(&p->model, Access::kPublic)},
        {"weight", FieldInfo(&p->frame_weight, Access::kPublic)},
        {"wheel_size", FieldInfo(&p->wheel_size_inch, Access::kPublic)},
        {"colors", FieldInfo(&p->colors, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
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
