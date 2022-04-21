#pragma once

#include "er/type_id.h"
#include "er/types/type_actions.h"

// injected
#include "../../data/pointers.h"

namespace er {

template <>
struct TypeActions<Pointers>;

template <>
TypeId TypeId::get(Pointers* /*unused*/);

}  // namespace er
