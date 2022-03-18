#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/complex_map.h"

namespace er {

template <>
struct TypeActions<ComplexMapVec>;

template <>
TypeId TypeId::get(ComplexMapVec* /*unused*/);

}  // namespace er
