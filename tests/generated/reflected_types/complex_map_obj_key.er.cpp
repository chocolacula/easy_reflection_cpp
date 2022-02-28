#include "complex_map_obj_key.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<ComplexMapObj::Key> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<ComplexMapObj::Key*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    if (p != nullptr) {
      map.insert({
        {"v1", FieldInfo(&p->v1, Access::kPublic)},
        {"v2", FieldInfo(&p->v2, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(ComplexMapObj::Key* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<ComplexMapObj::Key>::reflect,        //
                                                 &CommonActions<ComplexMapObj::Key>::type_name,    //
                                                 &CommonActions<ComplexMapObj::Key>::type_size,    //
                                                 &CommonActions<ComplexMapObj::Key>::call_new,     //
                                                 &CommonActions<ComplexMapObj::Key>::call_delete,  //
                                                 &CommonActions<ComplexMapObj::Key>::copy)));
  return id;
}

}  // namespace er

// clang-format on
