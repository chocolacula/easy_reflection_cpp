#pragma once

#include <cstddef>

template <typename T, typename... Args>
struct Sizeof {
  static constexpr size_t max() {
    return sizeof(T) > Sizeof<Args...>::max() ? sizeof(T) : Sizeof<Args...>::max();
  }
};

template <typename T>
struct Sizeof<T> {
  static constexpr size_t max() {
    return sizeof(T);
  }
};
