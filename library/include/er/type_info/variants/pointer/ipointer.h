#pragma once

#include "er/variable/var.h"

namespace er {

struct IPointer {
  virtual Expected<None> assign(Var var) = 0;
  virtual void unsafe_assign(void* ptr) = 0;
  virtual Var var() const = 0;
  virtual bool is_null() const = 0;
  virtual void init() = 0;
  virtual Expected<Var> get_nested() const = 0;
};

}  // namespace er