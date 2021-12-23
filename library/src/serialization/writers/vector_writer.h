#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "iwriter.h"

namespace er {

struct VectorWriter final : IWriter {
  VectorWriter() = delete;

  explicit VectorWriter(std::vector<uint8_t>* vector) : _vector(vector), _i(0) {
  }

  void write(const void* ptr, size_t bytes) override {
    _vector->resize(_vector->size() + bytes);
    std::memcpy(&(*_vector)[_i], ptr, bytes);
    _i += bytes;
  }

  void write(char ch) override {
    _vector->push_back(ch);
  }

  void step_back(size_t bytes) override {
    _vector->resize(_vector->size() - bytes);
  }

 private:
  std::vector<uint8_t>* _vector;
  size_t _i;
};

}  // namespace er