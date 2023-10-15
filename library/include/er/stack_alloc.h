#include <algorithm>
#include <memory>

namespace er {

template <typename T, size_t size>
class StackAlloc {
 public:
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  T* allocate(size_type n) {
    if (sizeof(T) * n > size) {
      return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    return reinterpret_cast<T*>(_stack_mem);
  }

  void deallocate(T* p, size_type n) {
    if (sizeof(T) * n > size) {
      ::operator delete(p);
    }
    // else do nothing, stack memory is automatically freed
  }

  StackAlloc() = default;
  StackAlloc(const StackAlloc&) = default;
  StackAlloc& operator=(const StackAlloc&) = default;
  StackAlloc(StackAlloc&&) noexcept = default;
  StackAlloc& operator=(StackAlloc&&) noexcept = default;
  ~StackAlloc() = default;

 private:
  alignas(T) uint8_t _stack_mem[size];
};

}  // namespace er
