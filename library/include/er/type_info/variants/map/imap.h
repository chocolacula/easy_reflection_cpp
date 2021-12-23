#pragma once

#include <functional>

#include "er/variable/var.h"

namespace er {

struct IMap {
  virtual Expected<None> assign(Var var) = 0;
  virtual void unsafe_assign(void* ptr) = 0;
  virtual Var own_var() const = 0;
  virtual TypeId key_type() const = 0;
  virtual TypeId val_type() const = 0;
  virtual void for_each(std::function<void(Var, Var)> callback) const = 0;
  virtual void for_each(std::function<void(Var, Var)> callback) = 0;
  virtual void unsafe_for_each(std::function<void(void*, void*)> callback) const = 0;
  virtual void clear() = 0;
  virtual size_t size() const = 0;
  virtual Expected<None> insert(Var key, Var value) = 0;
  virtual Expected<None> remove(Var key) = 0;
};

}  // namespace er