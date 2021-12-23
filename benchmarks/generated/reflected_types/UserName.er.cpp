#include "UserName.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile::UserName> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<UserProfile::UserName*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    map.insert({
      {"first_name", FieldInfo(&p->first_name, Access::kPublic)},
      {"second_name", FieldInfo(&p->second_name, Access::kPublic)},
      {"last_name", FieldInfo(&p->last_name, Access::kPublic)},
      
    });

    return Object(Var(p, is_const), std::move(map));
  }
};

template <>
TypeId TypeId::get(UserProfile::UserName* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<UserProfile::UserName>::reflect,        //
                                                 &CommonActions<UserProfile::UserName>::type_name,    //
                                                 &CommonActions<UserProfile::UserName>::type_size,    //
                                                 &CommonActions<UserProfile::UserName>::call_new,     //
                                                 &CommonActions<UserProfile::UserName>::call_delete,  //
                                                 &CommonActions<UserProfile::UserName>::copy)));
  return id;
}

}  // namespace er

// clang-format on
