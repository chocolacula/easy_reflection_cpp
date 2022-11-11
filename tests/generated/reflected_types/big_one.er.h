#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../../../tests/data/big_one.h"

namespace er {

template <>
struct TypeActions<BigOne>;

template <>
TypeId TypeId::get(BigOne* /*unused*/);

}  // namespace er
