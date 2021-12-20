#include "UserProfile.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile> {
  static TypeInfo reflect(void* value, bool is_const) {
    auto* p = static_cast<UserProfile*>(value);

    std::map<std::string_view, FieldInfo> map {
      
    };

    if (p != nullptr) {
      map.insert({
        {"user_id", FieldInfo(&p->user_id, Access::kPublic)},
        {"name", FieldInfo(&p->name, Access::kPublic)},
        {"homeworld", FieldInfo(&p->homeworld, Access::kPublic)},
        {"affiliation", FieldInfo(&p->affiliation, Access::kPublic)},
        {"profile_photo_url", FieldInfo(&p->profile_photo_url, Access::kPublic)},
        {"friends", FieldInfo(&p->friends, Access::kPublic)},
        {"last24hours_activity", FieldInfo(&p->last24hours_activity, Access::kPublic)},
        
      });
    }

    return Object(Var(p, is_const), std::move(map));
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
