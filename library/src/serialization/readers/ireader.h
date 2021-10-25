#pragma once

#include <cstddef>

namespace er {

struct IReader {
  virtual const void* read(size_t bytes) const = 0;
};

}  // namespace er