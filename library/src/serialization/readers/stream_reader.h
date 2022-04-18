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
    _stream.read(static_cast<char*>(buff), static_cast<std::streamsize>(size));
  }

  bool empty() const override {
    return _stream.peek() == EOF;
  }

 private:
  std::istream& _stream;
};

}  // namespace er