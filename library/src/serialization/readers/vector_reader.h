#pragma once

#include <cstddef>
#include <vector>

#include "ireader.h"

namespace er {
struct VectorReader final : IReader {
  VectorReader() = delete;

  explicit VectorReader(const std::vector<uint8_t>& vector) : _vector(vector), _i(0) {
  }

  const void* read(size_t bytes) const override {
    const void* ptr = &_vector[_i];
    _i += bytes;

    return ptr;
  }

 private:
  const std::vector<uint8_t>& _vector;
  mutable size_t _i;
};

}  // namespace er