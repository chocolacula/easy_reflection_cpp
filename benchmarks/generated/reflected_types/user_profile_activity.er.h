#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/user_profile.h"

namespace er {

template <>
struct TypeActions<UserProfile::Activity>;

template <>
TypeId TypeId::get(UserProfile::Activity* /*unused*/);

}  // namespace er
