#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/various.h"

namespace er {

template <>
struct TypeActions<Various::ComplexValue>;

template <>
TypeId TypeId::get(Various::ComplexValue* /*unused*/);

}  // namespace er
