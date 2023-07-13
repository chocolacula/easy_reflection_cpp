#include "config.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/type_info/variants/object/access.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Config> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Config*>(value);

    static std::map<std::string_view, FieldDesc> fields {
      {"compdb_dir", FieldDesc(&Config::compdb_dir, Access::kPublic)},
      {"templates", FieldDesc(&Config::templates, Access::kPublic)},
      {"input", FieldDesc(&Config::input, Access::kPublic)},
      {"output_dir", FieldDesc(&Config::output_dir, Access::kPublic)},
    };
    static std::map<std::string_view, MethodDesc> funcs {
    };
    return Object(Var(p, is_const), &fields, &funcs);
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
