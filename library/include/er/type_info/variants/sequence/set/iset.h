#pragma once

#include "../isequence.h"

namespace rr {

struct ISet : public ISequence {
  using ISequence::for_each;
  virtual Expected<None> remove(Var value) = 0;
  virtual bool contains(Var value) = 0;
};

}  // namespace rr