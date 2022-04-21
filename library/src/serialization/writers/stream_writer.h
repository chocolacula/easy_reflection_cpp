#pragma once

#include <cstddef>
#include <ostream>

#include "iwriter.h"

namespace er {

struct StreamWriter final : IWriter {
  StreamWriter() = delete;

  explicit StreamWriter(std::ostream& stream) : _stream(stream) {
  }

  void write(const void* ptr, size_t bytes) override {
    _stream.write(static_cast<const char*>(ptr), static_cast<std::streamsize>(bytes));
  }

  void write(uint8_t ch) override {
    _stream.put(ch);
  }

  uint8_t peek() const override {
    std::streamsize pos = _stream.tellp();
    return *static_cast<uint8_t*>(_stream.pword(pos));
  }

  void step_back(size_t bytes) override {
    std::streamsize pos = _stream.tellp();
    _stream.seekp(pos - static_cast<std::streamsize>(bytes));
  }

 private:
  std::ostream& _stream;
};

}  // namespace er