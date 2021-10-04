#pragma once

#include "../isequence.h"

namespace rr {

struct IQueue : public ISequence {
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual void pop() = 0;
  virtual Expected<Var> front() = 0;
  virtual Expected<Var> back() = 0;
};

}  // namespace rr