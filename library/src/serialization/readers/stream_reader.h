#pragma once

#include <cstddef>
#include <istream>

#include "ireader.h"

namespace rr {

template <unsigned int size>
struct StreamReader final : IReader {
  StreamReader() = delete;

  explicit StreamReader(std::istream& stream) : _stream(stream) {
  }

  const void* read(size_t bytes) const override {
    _stream.read(&_buff[0], static_cast<ssize_t>(bytes));
    return _buff;
  }

 private:
  std::istream& _stream;
  mutable char _buff[size];
};

}  // namespace rr