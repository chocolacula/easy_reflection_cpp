#include "user_profile.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<UserProfile*>(value);

    static std::map<std::string_view, FieldDesc> map {
      
      {"user_id", FieldDesc::create_member(value, Var(&p->user_id), Access::kPublic)},
      {"name", FieldDesc::create_member(value, Var(&p->name), Access::kPublic)},
      {"homeworld", FieldDesc::create_member(value, Var(&p->homeworld), Access::kPublic)},
      {"affiliation", FieldDesc::create_member(value, Var(&p->affiliation), Access::kPublic)},
      {"profile_photo_url", FieldDesc::create_member(value, Var(&p->profile_photo_url), Access::kPublic)},
      {"friends", FieldDesc::create_member(value, Var(&p->friends), Access::kPublic)},
      {"last24hours_activity", FieldDesc::create_member(value, Var(&p->last24hours_activity), Access::kPublic)},
      
    };

    return Object(Var(p, is_const), &map);
  }
};

template <>
TypeId TypeId::get(UserProfile* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<UserProfile>::reflect,        //
                                                 &CommonActions<UserProfile>::type_name,    //
                                                 &CommonActions<UserProfile>::type_size,    //
                                                 &CommonActions<UserProfile>::call_new,     //
                                                 &CommonActions<UserProfile>::call_delete,  //
                                                 &CommonActions<UserProfile>::copy)));
  return id;
}

}  // namespace er

// clang-format on
