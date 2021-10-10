#pragma once

#include <iomanip>
#include <memory>
#include <sstream>

#include "./float.h"
#include "ifloating.h"

namespace rr {

struct Floating : IFloating {
  Floating() = delete;

  template <typename T>
  Floating(T* value, bool is_const) : _floating(std::make_shared<Float<T>>(value, is_const)) {
  }

  Var var() override {
    return _floating->var();
  }

  double get() const override {
    return _floating->get();
  }

  Expected<None> set(double value) override {
    return _floating->set(value);
  }

  std::string to_string(int precision = 2) const override {
    return _floating->to_string(precision);
  }

 private:
  std::shared_ptr<IFloating> _floating;
};

}  // namespace rr