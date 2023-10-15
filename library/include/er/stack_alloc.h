#include <memory>
#include <new>
#include <algorithm>

namespace er {

template <typename T, size_t size>
class StackAlloc {
public:
    using value_type = T;
    using pointer = T*;
}
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    pointer allocate(size_type n) {
        if (sizeof(T) * n > size) {
            return static_cast<pointer>(::operator new(n * sizeof(T)));
        } else {
            return reinterpret_cast<pointer>(_stack);
        }
    }

    void deallocate(pointer p, size_type n) {
        if (sizeof(T) * n > size) {
            ::operator delete(p);
        } else {
            // do nothing, stack memory is automatically freed
        }
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    size_type max_size() const noexcept {
        return std::max<size_type>(1, size / sizeof(T));
    }

    pointer address(reference x) const noexcept {
        return std::addressof(x);
    }

    const T* address(const_reference x) const noexcept {
        return std::addressof(x);
    }

    StackAlloc() = default;
    StackAlloc(const StackAlloc&) = default;
    StackAlloc& operator=(const StackAlloc&) = default;
    StackAlloc(StackAlloc&&) = default;
    StackAlloc& operator=(StackAlloc&&) = default;
    ~StackAlloc() = default;

private:
    alignas(T) char _stack[size];
};

}  // namespace er
