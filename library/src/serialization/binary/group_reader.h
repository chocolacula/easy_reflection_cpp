#pragma once

#include <cfloat>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "../readers/ireader.h"

namespace er {

struct GroupReader {
  GroupReader(const GroupReader& other) = delete;

  explicit GroupReader(IReader* reader)  //
      : _reader(reader), _word(0) {
    _reader->read(&_header, 1);
  }

  int64_t read_signeg() const {
    auto neg = is_negative();
    auto value = read_one();

    return static_cast<int64_t>(neg ? -value : value);
  }

  uint64_t read_unsigned() const {
    return read_one();
  }

  double read_float() const {
    auto u_value = read_one();

    // check the first half for emptyness
    if (reinterpret_cast<uint32_t*>(&u_value)[1] == 0) {
      float f_value;
      std::memcpy(&f_value, &u_value, sizeof(f_value));
      return f_value;
    }

    double value;
    std::memcpy(&value, &u_value, sizeof(value));
    return value;
  }

  std::string read_string() const {

    // get size from the reader
    size_t size = 0;
    read_data(&size);

    std::string str;
    str.resize(size);

    _reader->read(str.data(), size);

    _reader->read(&_header, 1);
    _word = 0;

    return str;
  }

  bool is_null() const {
    uint32_t v = 0;
    _reader->peek(&v, sizeof(v));
    return v == kNull;
  }

 private:
  const uint32_t kNull = 0x6E756C6C;  // n(6E) u(75) l(6C) l(6C)

  IReader* _reader;

  mutable uint8_t _header;
  mutable unsigned int _word;

  uint64_t read_one() const {
    uint64_t value = 0;
    read_data(&value);

    // read next word in the header
    _word++;
    if (_word > 1 && !_reader->empty()) {
      _reader->read(&_header, 1);
      _word = 0;
    }

    return value;
  }

  // read data from reader, do not touch the header
  inline void read_data(void* ptr) const {
    auto chunks = _header;
    chunks >>= 4U * (1 - _word);
    chunks &= 0b00000111U;

    _reader->read(ptr, chunks + 1);
  }

  inline bool is_negative() const {
    auto negative_bit = _header;
    negative_bit >>= (4U * (1U - _word) + 3U);
    negative_bit &= 0b00000001U;

    return negative_bit == 1;
  }
};

}  // namespace er