#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

#include "alloc.h"

namespace er {

struct BuffAlloc final : public palloc_t {
  BuffAlloc() : kSize(0) {
  }

  template <size_t size>
  explicit BuffAlloc(std::array<uint8_t, size>* arr) : _buff(arr->data()), kSize(size) {
  }

  BuffAlloc(uint8_t* p, size_t size) : _buff(p), kSize(size) {
  }

  uint8_t* allocate(size_t n) {
    if (n > kSize) {
      return palloc_t::allocate(n);
    }
    return static_cast<uint8_t*>(_buff);
  }

  void deallocate(uint8_t* p, size_t n) {
    if (n > kSize) {
      palloc_t::deallocate(p, n);
    }
    // else do nothing, stack memory is automatically freed
  }

 private:
  uint8_t* _buff;
  const size_t kSize;
};

}  // namespace er
