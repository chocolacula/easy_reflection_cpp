#pragma once

#include <iostream>

#include "er/tools/format.h"

inline void println() {
  std::cout << std::endl;
}

template <typename T>
inline void println(T&& arg) {
  std::cout << er::format("{}", std::forward<T>(arg)) << std::endl;
}

template <typename... Ts>
inline void println(std::string_view fmt, Ts&&... args) {
  std::cout << er::format(fmt, std::forward<Ts>(args)...) << std::endl;
}
