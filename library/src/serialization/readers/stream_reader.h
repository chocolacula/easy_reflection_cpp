#pragma once

#include <cstddef>
#include <istream>

#include "ireader.h"

namespace er {

struct StreamReader final : IReader {
  StreamReader() = delete;

  explicit StreamReader(std::istream& stream) : _stream(stream) {
  }

  void read(void* buff, size_t size) const override {
    _stream.read(static_cast<char*>(buff), static_cast<ssize_t>(size));
  }

 private:
  std::istream& _stream;
};

}  // namespace er