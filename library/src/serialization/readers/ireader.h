#pragma once

#include <cstddef>

namespace er {

struct IReader {
  virtual void read(void* buff, size_t size) const = 0;
  virtual bool empty() const = 0;
};

}  // namespace er