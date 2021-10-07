#pragma once

#include <memory>

#include "std_set.h"
#include "std_unordered_set.h"

namespace rr {

struct Set : public ISet {
  Set() = delete;

  template <typename T>
  explicit Set(std::set<T>* set, bool is_const) : _set(std::make_shared<StdSet<T>>(set, is_const)) {
  }

  template <typename T>
  Set(std::unordered_set<T>* set, bool is_const) : _set(std::make_shared<StdUnorderedSet<T>>(set, is_const)) {
  }

  Var own_var() const override {
    return _set->own_var();
  }

  TypeId nested_type() const override {
    return _set->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const override {
    _set->for_each(callback);
  }

  void clear() override {
    _set->clear();
  }

  size_t size() const override {
    return _set->size();
  }

  Expected<None> push(Var value) override {
    return _set->push(value);
  }

  Expected<None> remove(Var value) override {
    return _set->remove(value);
  }

  bool contains(Var value) override {
    return _set->contains(value);
  }

 private:
  std::shared_ptr<ISet> _set;
};

}  // namespace rr
