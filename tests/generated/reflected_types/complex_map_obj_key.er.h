#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/complex_map.h"

namespace er {

template <>
struct TypeActions<ComplexMapObj::Key>;

template <>
TypeId TypeId::get(ComplexMapObj::Key* /*unused*/);

}  // namespace er
