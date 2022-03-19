#include "er/tools/stringify.h"

#include <iomanip>
#include <sstream>

namespace er {

std::string to_string(bool value) {
  return value ? "true" : "false";
}

std::string to_string(int64_t value) {
  return std::to_string(value);
}

std::string to_string(uint64_t value) {
  return std::to_string(value);
}

std::string to_string(double value, int precision) {
  std::stringstream stream;
  stream << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << value;

  return stream.str();
}

}  // namespace er