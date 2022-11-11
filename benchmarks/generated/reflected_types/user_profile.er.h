#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../../../benchmarks/data/user_profile.h"

namespace er {

template <>
struct TypeActions<UserProfile>;

template <>
TypeId TypeId::get(UserProfile* /*unused*/);

}  // namespace er
