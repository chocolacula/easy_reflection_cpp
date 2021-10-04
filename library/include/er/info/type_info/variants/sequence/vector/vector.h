#pragma once

#include <memory>

#include "std_vector.h"

namespace rr {

struct Vector : public IVector {
  Vector() = delete;

  template <typename T>
  explicit Vector(std::vector<T>* vector, bool is_const) : _vector(std::make_shared<StdVector<T>>(vector, is_const)) {
  }

  Var own_var() const override {
    return _vector->own_var();
  }

  TypeId nested_type() const override {
    return _vector->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const override {
    static_cast<const ISequence*>(_vector.get())->for_each(callback);
  }

  void for_each(std::function<void(Var)> callback) override {
    _vector->for_each(callback);
  }

  void clear() override {
    _vector->clear();
  }

  size_t size() const override {
    return _vector->size();
  }

  Expected<None> push(Var value) override {
    return _vector->push(value);
  }

  void pop() override {
    _vector->pop();
  }

  Expected<Var> front() override {
    return _vector->front();
  };

  Expected<Var> back() override {
    return _vector->back();
  };

  Expected<Var> at(size_t idx) override {
    return _vector->at(idx);
  }

  Expected<Var> operator[](size_t idx) override {
    return _vector->operator[](idx);
  }

 private:
  std::shared_ptr<IVector> _vector;
};

}  // namespace rr
