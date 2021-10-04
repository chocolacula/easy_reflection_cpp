#pragma once

#include <functional>

#include "er/variable/var.h"

namespace rr {

struct ISequence {
  virtual Var own_var() const = 0;
  virtual TypeId nested_type() const = 0;
  virtual void for_each(std::function<void(Var)> callback) const = 0;
  virtual void clear() = 0;
  virtual size_t size() const = 0;
  virtual Expected<None> push(Var value) = 0;
};

}  // namespace rr