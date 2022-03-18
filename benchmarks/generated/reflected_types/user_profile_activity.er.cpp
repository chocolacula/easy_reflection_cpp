#include "user_profile_activity.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile::Activity> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<UserProfile::Activity*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"photo_url", FieldDesc::create_member(value, Var(&p->photo_url), Access::kPublic)},
      {"grade", FieldDesc::create_member(value, Var(&p->grade), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(UserProfile::Activity* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<UserProfile::Activity>::reflect,        //
                                                 &CommonActions<UserProfile::Activity>::type_name,    //
                                                 &CommonActions<UserProfile::Activity>::type_size,    //
                                                 &CommonActions<UserProfile::Activity>::call_new,     //
                                                 &CommonActions<UserProfile::Activity>::call_delete,  //
                                                 &CommonActions<UserProfile::Activity>::copy)));
  return id;
}

}  // namespace er

// clang-format on