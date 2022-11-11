#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../../../example/data/colors.h"

namespace er {

template <>
struct TypeActions<Colors>;

template <>
TypeId TypeId::get(Colors* /*unused*/);

}  // namespace er
