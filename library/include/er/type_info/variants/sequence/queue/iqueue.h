#pragma once

#include "../isequence.h"

namespace er {

struct IQueue : public ISequence {
  using ISequence::for_each;
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual void pop() = 0;
  virtual Expected<Var> front() = 0;
  virtual Expected<Var> back() = 0;
};

}  // namespace er