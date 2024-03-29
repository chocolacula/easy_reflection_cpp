#pragma once

#include <string_view>

#include "er/error/error.h"
#include "er/type_id.h"

using namespace er;

namespace er::sequence {

/// move error formating functionality outside sequence template classes
class ErrHelper {
 protected:
  static Error error(std::string_view message, TypeId type, TypeId nested_type);
};

}  // namespace er::sequence