#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../config.h"

namespace er {

template <>
struct TypeActions<Config::Templates>;

template <>
TypeId TypeId::get(Config::Templates* /*unused*/);

}  // namespace er
