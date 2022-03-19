#pragma once

#include <iomanip>
#include <sstream>

#include "er/expected.h"
#include "er/variable/var.h"

namespace er {

struct IFloating {
  virtual Expected<None> assign(Var var) = 0;
  virtual void unsafe_assign(void* ptr) = 0;
  virtual Var var() = 0;
  virtual size_t size() = 0;
  virtual double get() const = 0;
  virtual Expected<None> set(double value) = 0;
};

}  // namespace er