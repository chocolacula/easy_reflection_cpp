#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/big.h"

namespace er {

template <>
struct TypeActions<Big>;

template <>
TypeId TypeId::get(Big* /*unused*/);

}  // namespace er
