#include "Grade.er.h"

// clang-format off
#include "er/reflection/the_great_table.h"
#include "er/types/common_actions.h"
#include "er/types/type_actions.h"

namespace er {

template <>
struct TypeActions<UserProfile::Grade> {
  static TypeInfo reflect(void* value, bool is_const) {

    static constexpr ConstexprMap<UserProfile::Grade, 2> constants{
    // be aware of use sequential '{' with template engines
    {
      {
        {UserProfile::Grade::kLiked, "kLiked"},
        {UserProfile::Grade::kDisliked, "kDisliked"},
        
    }}};

    return Enum(static_cast<UserProfile::Grade*>(value), is_const, constants);
  }
};

template <>
TypeId TypeId::get(UserProfile::Grade* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<UserProfile::Grade>::reflect,        //
                                                 &CommonActions<UserProfile::Grade>::type_name,    //
                                                 &CommonActions<UserProfile::Grade>::type_size,    //
                                                 &CommonActions<UserProfile::Grade>::call_new,     //
                                                 &CommonActions<UserProfile::Grade>::call_delete,  //
                                                 &CommonActions<UserProfile::Grade>::copy)));
  return id;
}

}  // namespace er

// clang-format on
