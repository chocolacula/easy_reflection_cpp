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
      
      {"user_id", FieldDesc::create_member(p, &p->user_id, FieldAttributes::kPublic)},
      {"name", FieldDesc::create_member(p, &p->name, FieldAttributes::kPublic)},
      {"homeworld", FieldDesc::create_member(p, &p->homeworld, FieldAttributes::kPublic)},
      {"affiliation", FieldDesc::create_member(p, &p->affiliation, FieldAttributes::kPublic)},
      {"profile_photo_url", FieldDesc::create_member(p, &p->profile_photo_url, FieldAttributes::kPublic)},
      {"friends", FieldDesc::create_member(p, &p->friends, FieldAttributes::kPublic)},
      {"last24hours_activity", FieldDesc::create_member(p, &p->last24hours_activity, FieldAttributes::kPublic)},
      
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
