#pragma once

#include <array>
#include <cfloat>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

#include "../writers/iwriter.h"

namespace er {

struct GroupWriter {
  GroupWriter(const GroupWriter& other) = delete;

  explicit GroupWriter(IWriter* writer)  //
      : _writer(writer), _i(1), _word(0) {
    _group[0] = 0;
  }

  ~GroupWriter() {
    if (_i > 1) {  // _i == 1 means only header has been written
      _writer->write(&_group[0], _i);
    }
  }

  void write(bool value) {
    write_one(static_cast<uint64_t>(value), false);
  }

  void write(size_t value) {
    write_one(value, false);
  }

  void write(std::string_view str) {
    // write string size to the group
    write_one(str.size(), false);

    // write the group to the stream if it wasn't while writing size
    if (_word != 0) {
      flush_header();
    }

    // write the string to the stream
    _writer->write(str.data(), str.size());
  }

  void write(const void* ptr, size_t size, bool is_signed) {
    uint64_t value = 0;

    if (is_signed) {
      std::memcpy(&value, ptr, size);

      auto aligned_zeroes = 64U - size * 8;
      bool neg = ((value << aligned_zeroes) & 0x8000000000000000) != 0;

      if (neg) {
        value <<= aligned_zeroes;
        value = ~value;
        value >>= aligned_zeroes;
        value += 1;
      }

      write_one(value, neg);
    } else {
      std::memcpy(&value, ptr, size);
      write_one(value, false);
    }
  }

  void write(double value) {
    uint64_t u_value = 0;

    if (value >= -FLT_MAX && value <= FLT_MAX) {
      float f_value = value;
      std::memcpy(&u_value, &f_value, sizeof(f_value));
    } else {
      std::memcpy(&u_value, &value, sizeof(value));
    }

    write_one(u_value, false);
  }

 private:
  IWriter* _writer;
  std::array<uint8_t, 17> _group;
  size_t _i;

  unsigned int _word;

  void write_one(uint64_t value, bool neg) {
    // set sign bit
    _group[0] |= (static_cast<uint8_t>(neg) << 3U);

    auto zeroes = __builtin_clzll(value);
    auto chunks = (64U - zeroes);
    chunks = chunks / 8 - static_cast<uint8_t>((chunks % 8) == 0);

    // set number of bytes
    _group[0] |= chunks;
    // shift if needed
    _group[0] <<= (1U - _word) * 4;

    auto* p = reinterpret_cast<uint8_t*>(&value);
    std::memcpy(&_group[_i], p, chunks + 1);
    _i += chunks + 1;

    // get next word in the header, push one if needed
    _word++;
    if (_word > 1) {
      flush_header();
    }
  }

  inline void flush_header() {
    _writer->write(&_group[0], _i);
    _group[0] = 0;
    _i = 1;
    _word = 0;
  }

  inline bool is_negative() {
    auto negative_bit = _group[0];
    negative_bit >>= (4U * (1U - _word) + 3U);
    negative_bit &= 0b00000001U;

    return negative_bit == 1;
  }
};

}  // namespace er