#pragma once

#include <iomanip>
#include <sstream>
#include <cstdint>

namespace er {

std::string to_string(bool value);
std::string to_string(int64_t value);
std::string to_string(uint64_t value);
std::string to_string(double value, int precision);

}  // namespace er
