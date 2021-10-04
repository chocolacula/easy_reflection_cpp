#pragma once

#include <iostream>

#include "er/tools/format.h"

void println() {
  std::cout << std::endl;
}

template <typename... Ts>
void println(std::string_view fmt, Ts&&... args) {
  std::cout << rr::format(fmt, std::forward<Ts>(args)...) << std::endl;
}
