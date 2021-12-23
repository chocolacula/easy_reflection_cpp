#pragma once

#include <iomanip>
#include <memory>
#include <sstream>

#include "./float.h"
#include "ifloating.h"

namespace er {

struct Floating : IFloating {
  Floating() = delete;

  template <typename T>
  Floating(T* value, bool is_const) : _floating(std::make_shared<Float<T>>(value, is_const)) {
  }

  Expected<None> assign(Var var) override {
    return _floating->assign(var);
  }

  void unsafe_assign(void* ptr) override {
    _floating->unsafe_assign(ptr);
  }

  Var var() override {
    return _floating->var();
  }

  size_t size() override {
    return _floating->size();
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

}  // namespace er