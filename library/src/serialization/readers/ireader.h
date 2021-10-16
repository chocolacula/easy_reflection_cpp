#pragma once

#include <cstddef>

namespace rr {

struct IReader {
  virtual const void* read(size_t bytes) const = 0;
};

}  // namespace rr