#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../../../example/data/bicycle.h"

namespace er {

template <>
struct TypeActions<Bicycle>;

template <>
TypeId TypeId::get(Bicycle* /*unused*/);

}  // namespace er
