#include <algorithm>
#include <array>
#include <cstdint>
#include <memory_resource>

namespace er {

struct BuffAlloc final : public std::pmr::polymorphic_allocator<uint8_t> {
  template <size_t size>
  BuffAlloc(std::array<uint8_t, size>* arr) : _buff(arr->data()), kSize(size) {
  }

  BuffAlloc(uint8_t* p, size_t size) : _buff(p), kSize(size) {
  }

  uint8_t* allocate(size_t n) {
    if (n > kSize) {
      return std::pmr::polymorphic_allocator<uint8_t>::allocate(n);
    }
    return static_cast<uint8_t*>(_buff);
  }

  void deallocate(uint8_t* p, size_t n) {
    if (n > kSize) {
      std::pmr::polymorphic_allocator<uint8_t>::deallocate(p, n);
    }
    // else do nothing, stack memory is automatically freed
  }

 private:
  uint8_t* _buff;
  const size_t kSize;
};

}  // namespace er
