#pragma once

#include <iostream>

#include "er/tools/format.h"

void println() {
  std::cout << std::endl;
}

template <typename T>
void println(T&& arg) {
  std::cout << er::format("{}", std::forward<T>(arg)) << std::endl;
}

template <typename... Ts>
void println(std::string_view fmt, Ts&&... args) {
  std::cout << er::format(fmt, std::forward<Ts>(args)...) << std::endl;
}
