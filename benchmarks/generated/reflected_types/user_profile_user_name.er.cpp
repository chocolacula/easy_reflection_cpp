#include "user_profile_user_name.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile::UserName> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<UserProfile::UserName*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"first_name", FieldDesc::create_member(p, &p->first_name, FieldAttributes::kPublic)},
      {"second_name", FieldDesc::create_member(p, &p->second_name, FieldAttributes::kPublic)},
      {"last_name", FieldDesc::create_member(p, &p->last_name, FieldAttributes::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
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
