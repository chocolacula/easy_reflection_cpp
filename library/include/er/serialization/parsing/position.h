#pragma once

#include <cstddef>
#include <string>

#include "er/tools/format.h"

namespace rr {

struct Position {
  size_t column = 0;
  size_t line_number = 0;

  std::string to_string() {
    return format("ln:{} col:{}", line_number + 1, column + 1);
  }
};

}  // namespace rr
