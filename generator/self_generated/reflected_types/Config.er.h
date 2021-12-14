#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../config.h"

namespace er {

template <>
struct TypeActions<Config>;

template <>
TypeId TypeId::get(Config* /*unused*/);

}  // namespace er
