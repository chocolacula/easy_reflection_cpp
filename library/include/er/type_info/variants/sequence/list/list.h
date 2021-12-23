#pragma once

#include <memory>

#include "std_deque.h"
#include "std_list.h"

namespace er {

struct List : public IList {
  List() = delete;

  template <typename T>
  List(std::list<T>* list, bool is_const) : _list(std::make_shared<StdList<T>>(list, is_const)) {
  }

  template <typename T>
  List(std::deque<T>* deque, bool is_const) : _list(std::make_shared<StdDeque<T>>(deque, is_const)) {
  }

  Expected<None> assign(Var var) override {
    return _list->assign(var);
  }

  void unsafe_assign(void* ptr) override {
    _list->unsafe_assign(ptr);
  }

  Var own_var() const override {
    return _list->own_var();
  }

  TypeId nested_type() const override {
    return _list->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const override {
    static_cast<const ISequence*>(_list.get())->for_each(callback);
  }

  void for_each(std::function<void(Var)> callback) override {
    _list->for_each(callback);
  }

  void unsafe_for_each(std::function<void(void*)> callback) const override {
    _list->unsafe_for_each(callback);
  }

  void clear() override {
    _list->clear();
  }

  size_t size() const override {
    return _list->size();
  }

  Expected<None> push(Var value) override {
    return _list->push(value);
  }

  void pop() override {
    _list->pop();
  }

  void push_front(Var element) override {
    _list->push_front(element);
  }

  void pop_front() override {
    _list->pop_front();
  }

  Expected<Var> front() override {
    return _list->front();
  }

  Expected<Var> back() override {
    return _list->back();
  }

 private:
  std::shared_ptr<IList> _list;
};

}  // namespace er
