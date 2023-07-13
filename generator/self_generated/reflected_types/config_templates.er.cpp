#include "config_templates.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<Config::Templates> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<Config::Templates*>(value);

    static std::map<std::string_view, FieldDesc> fields {
      {"header", FieldDesc(&Config::Templates::header, Access::kPublic)},
      {"enum", FieldDesc(&Config::Templates::for_enum, Access::kPublic)},
      {"object", FieldDesc(&Config::Templates::object, Access::kPublic)},
    };
    static std::map<std::string_view, MethodDesc> methods {
    };
    return Object(Var(p, is_const), &fields, &methods);
  }
};

template <>
TypeId TypeId::get(Config::Templates* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config::Templates>::reflect,        //
                                                 &CommonActions<Config::Templates>::type_name,    //
                                                 &CommonActions<Config::Templates>::type_size,    //
                                                 &CommonActions<Config::Templates>::call_new,     //
                                                 &CommonActions<Config::Templates>::call_delete,  //
                                                 &CommonActions<Config::Templates>::copy,         //
                                                 &CommonActions<Config::Templates>::move)));
  return id;
}

}  // namespace er

// clang-format on
