#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "ireader.h"

namespace er {
struct VectorReader final : IReader {
  VectorReader() = delete;

  explicit VectorReader(const std::vector<uint8_t>& vector) : _vector(vector), _i(0) {
  }

  void read(void* buff, size_t size) const override {
    std::memcpy(buff, &_vector[_i], size);
    _i += size;
  }

  void peek(void* buff, size_t size) const override {
    std::memcpy(buff, &_vector[_i], size);
  }

  bool empty() const override {
    return _i >= _vector.size();
  }

 private:
  const std::vector<uint8_t>& _vector;
  mutable size_t _i;
};

}  // namespace er