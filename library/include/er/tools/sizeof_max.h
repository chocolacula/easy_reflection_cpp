#pragma once

#include <cstddef>

template <typename T>
constexpr size_t sizeof_max(const T& /*unused*/) {
  return sizeof(T);
}

template <typename T, typename... Args>
constexpr size_t sizeof_max(const T& /**/, Args... args) {
  return sizeof(T) > sizeof_max(args...) ? sizeof(T) : sizeof_max(args...);
}
