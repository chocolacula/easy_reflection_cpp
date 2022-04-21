#pragma once

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

  void peek(void* buff, size_t size) const override {
    _stream.read(static_cast<char*>(buff), static_cast<std::streamsize>(size));
    _stream.seekg(-static_cast<std::streamoff>(size), std::ios_base::cur);
  }

  bool empty() const override {
    return _stream.peek() == EOF;
  }

 private:
  std::istream& _stream;
};

}  // namespace er