#pragma once

#include <cstddef>

namespace rr {

struct IWriter {
  virtual void write(const void* ptr, size_t bytes) = 0;
  virtual void write(char ch) = 0;
  virtual void step_back(size_t bytes) = 0;
};

}  // namespace rr