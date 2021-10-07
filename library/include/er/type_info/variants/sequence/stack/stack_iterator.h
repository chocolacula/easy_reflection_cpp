#pragma once

#include <stack>

template <typename T>
struct StackIterator : public std::stack<T> {
  static typename decltype(StackIterator::c)::iterator begin(std::stack<T>* stack) {
    return (stack->*&StackIterator::c).begin();
  }

  static typename decltype(StackIterator::c)::iterator end(std::stack<T>* stack) {
    return (stack->*&StackIterator::c).end();
  }

  template <typename SameT>
  friend class StdStack;
};