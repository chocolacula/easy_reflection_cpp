#pragma once

#include <cfloat>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "../readers/ireader.h"

namespace er {

struct GroupReader {
  GroupReader(const GroupReader& other) = delete;

  explicit GroupReader(IReader* reader)  //
      : _reader(reader), _word(0) {
    _header = *static_cast<const uint8_t*>(_reader->read(1));
  }

  int64_t read_integer() const {
    auto neg = is_negative();
    auto value = read_one();

    return static_cast<int64_t>(neg ? -value : value);
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

 private:
  IReader* _reader;

  mutable uint8_t _header;
  mutable unsigned int _word;

  uint64_t read_one() const {
    uint64_t value = 0;
    auto chunks = _header;
    chunks >>= 4U * (1 - _word);
    chunks &= 0b00000111U;

    const auto* p = _reader->read(chunks + 1);
    std::memcpy(&value, p, chunks + 1);

    // read next word in the header
    _word++;
    if (_word > 1) {
      _header = *static_cast<const uint8_t*>(_reader->read(1));
      _word = 0;
    }

    return value;
  }

  inline bool is_negative() const {
    auto negative_bit = _header;
    negative_bit >>= (4U * (1U - _word) + 3U);
    negative_bit &= 0b00000001U;

    return negative_bit == 1;
  }
};

}  // namespace er