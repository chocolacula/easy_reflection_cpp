#pragma once

#include <iomanip>
#include <sstream>

#include "er/expected.h"
#include "er/variable/var.h"

namespace rr {

struct IFloating {
  virtual Var var() = 0;
  virtual double get() const = 0;
  virtual Expected<None> set(double value) = 0;
  virtual std::string to_string(int precision) const = 0;
};

}  // namespace rr