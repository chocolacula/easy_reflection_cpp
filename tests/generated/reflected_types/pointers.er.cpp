#include "pointers.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Pointers> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Pointers*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"obj", FieldDesc::create_member(p, &p->obj, FieldAttributes::kPublic)},
      {"null_ptr", FieldDesc::create_member(p, &p->null_ptr, FieldAttributes::kPublic)},
      {"arr", FieldDesc::create_member(p, &p->arr, FieldAttributes::kPublic)},
      {"vec", FieldDesc::create_member(p, &p->vec, FieldAttributes::kPublic)},
      {"map", FieldDesc::create_member(p, &p->map, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(Pointers* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Pointers>::reflect,        //
                                                 &CommonActions<Pointers>::type_name,    //
                                                 &CommonActions<Pointers>::type_size,    //
                                                 &CommonActions<Pointers>::call_new,     //
                                                 &CommonActions<Pointers>::call_delete,  //
                                                 &CommonActions<Pointers>::copy)));
  return id;
}

}  // namespace er

// clang-format on
