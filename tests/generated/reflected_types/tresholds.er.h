#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/tresholds.h"

namespace er {

template <>
struct TypeActions<Tresholds>;

template <>
TypeId TypeId::get(Tresholds* /*unused*/);

}  // namespace er
