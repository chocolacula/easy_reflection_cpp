#pragma once

#include "er/type_id.h"

namespace er::reflection {

std::string_view type_name(TypeId id);

}  // namespace er::reflection