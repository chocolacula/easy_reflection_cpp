#include "config.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/type_info/variants/object/field_attributes.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Config> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Config*>(value);

    static std::map<std::string_view, FieldDesc> map {
      {"compdb_dir", FieldDesc::create_member(p, &p->compdb_dir, FieldAttributes::kPublic)},
      {"templates", FieldDesc::create_member(p, &p->templates, FieldAttributes::kPublic)},
      {"input", FieldDesc::create_member(p, &p->input, FieldAttributes::kPublic)},
      {"output_dir", FieldDesc::create_member(p, &p->output_dir, FieldAttributes::kPublic)},

    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(Config* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config>::reflect,        //
                                                 &CommonActions<Config>::type_name,    //
                                                 &CommonActions<Config>::type_size,    //
                                                 &CommonActions<Config>::call_new,     //
                                                 &CommonActions<Config>::call_delete,  //
                                                 &CommonActions<Config>::copy,         //
                                                 &CommonActions<Config>::move)));
  return id;
}

}  // namespace er

// clang-format on
