#pragma once

//clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

// injected
#include "../../config.h"

namespace er {

template <>
struct TypeActions<Config> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Config*>(value);

    std::map<std::string_view, FieldInfo> map{

    };

    if (p != nullptr) {
      map.insert({
          {"compdb_dir", FieldInfo(&p->compdb_dir, Access::kPublic)},
          {"templates", FieldInfo(&p->templates, Access::kPublic)},
          {"input", FieldInfo(&p->input, Access::kPublic)},
          {"output_dir", FieldInfo(&p->output_dir, Access::kPublic)},

      });
    }

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(Config* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config>::reflect,        //
                                                 &CommonActions<Config>::type_name,    //
                                                 &CommonActions<Config>::type_size,    //
                                                 &CommonActions<Config>::call_new,     //
                                                 &CommonActions<Config>::call_delete,  //
                                                 &CommonActions<Config>::copy)));
  return id;
}

}  // namespace er

//clang-format on
