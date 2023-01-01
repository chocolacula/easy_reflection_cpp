#pragma once

#include "../isequence.h"

namespace er {

struct IStack : public ISequence {
  ~IStack() override = default;

  using ISequence::for_each;
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual void pop() = 0;
  virtual Expected<Var> top() = 0;
};

}  // namespace er
