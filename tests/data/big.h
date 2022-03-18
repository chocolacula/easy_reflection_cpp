#pragma once

#include <cstdint>
#include <string>

#include "er/attributes.h"

/// has size more then Box can optimize in a stack memory
ER_REFLECT()
struct Big {
  uint64_t int1;
  uint64_t int2;
  uint64_t int3;
  uint64_t int4;
  std::string str1;
  std::string str2;
  std::string str3;
  std::string str4;
};