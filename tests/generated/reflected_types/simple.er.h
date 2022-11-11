#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../../../tests/data/simple.h"

namespace er {

template <>
struct TypeActions<Simple>;

template <>
TypeId TypeId::get(Simple* /*unused*/);

}  // namespace er
