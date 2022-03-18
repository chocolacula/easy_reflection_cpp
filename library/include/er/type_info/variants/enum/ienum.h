#pragma once

#include "er/variable/var.h"

namespace er {

struct IEnum {
  virtual Expected<None> assign(Var var) = 0;
  virtual void unsafe_assign(void* ptr) = 0;
  virtual Var var() const = 0;
  virtual std::string_view to_string() const = 0;
  virtual Expected<None> parse(std::string_view name) = 0;
};

}  // namespace er
