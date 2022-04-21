#pragma once

#include <cstddef>
#include <cstdint>

namespace er {

struct IWriter {
  virtual void write(const void* ptr, size_t bytes) = 0;
  virtual void write(uint8_t ch) = 0;
  virtual uint8_t peek() const = 0;
  virtual void step_back(size_t bytes) = 0;
};

}  // namespace er