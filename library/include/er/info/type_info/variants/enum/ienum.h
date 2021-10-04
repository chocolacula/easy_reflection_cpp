#pragma once

#include "er/variable/var.h"

namespace rr {

struct IEnum {

  virtual Var var() const = 0;
  virtual std::string_view to_string() const = 0;
  virtual Expected<None> parse(std::string_view name) = 0;
};

}  // namespace rr
