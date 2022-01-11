#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/various.h"

namespace er {

template <>
struct TypeActions<Various>;

template <>
TypeId TypeId::get(Various* /*unused*/);

}  // namespace er
