#pragma once

#include <functional>

#include "er/variable/var.h"

namespace rr {

struct IArray {
  virtual Var own_var() const = 0;
  virtual TypeId nested_type() const = 0;
  virtual void for_each(std::function<void(Var)> callback) const = 0;
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual size_t size() const = 0;
  virtual Expected<Var> front() = 0;
  virtual Expected<Var> back() = 0;
  virtual Expected<Var> at(size_t idx) = 0;
  virtual Expected<Var> operator[](size_t idx) = 0;
  virtual Expected<None> fill(Var filler) = 0;
};

}  // namespace rr
