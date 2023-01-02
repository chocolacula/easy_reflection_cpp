#pragma once

#include "er/type_info/variants/sequence/isequence.h"
#include "list/std_deque.h"
#include "list/std_list.h"
#include "queue/std_queue.h"
#include "set/std_set.h"
#include "set/std_unordered_set.h"
#include "stack/std_stack.h"
#include "vector/std_vector.h"

namespace er {

struct Sequence {
  Sequence() = delete;

  template <typename T>
  Sequence(std::vector<T>* vector, bool is_const) {
    new (_mem) StdVector<T>(vector, is_const);
  }

  template <typename T>
  Sequence(std::list<T>* list, bool is_const) {
    new (_mem) StdList<T>(list, is_const);
  }

  template <typename T>
  Sequence(std::deque<T>* deque, bool is_const) {
    new (_mem) StdDeque<T>(deque, is_const);
  }

  template <typename T>
  Sequence(std::stack<T>* stack, bool is_const) {
    new (_mem) StdStack<T>(stack, is_const);
  }

  template <typename T>
  Sequence(std::queue<T>* queue, bool is_const) {
    new (_mem) StdQueue<T>(queue, is_const);
  }

  template <typename T>
  Sequence(std::set<T>* set, bool is_const) {
    new (_mem) StdSet<T>(set, is_const);
  }

  template <typename T>
  Sequence(std::unordered_set<T>* set, bool is_const) {
    new (_mem) StdUnorderedSet<T>(set, is_const);
  }

  ~Sequence() {
    impl()->~ISequence();
  }

  Expected<None> assign(Var var) {
    return impl()->assign(var);
  }

  void unsafe_assign(void* ptr) {
    return impl()->unsafe_assign(ptr);
  }

  Var own_var() const {
    return impl()->own_var();
  }

  TypeId nested_type() const {
    return impl()->nested_type();
  }

  void for_each(std::function<void(Var)> callback) const {
    impl()->for_each(callback);
  }

  void unsafe_for_each(std::function<void(void*)> callback) const {
    impl()->unsafe_for_each(callback);
  }

  void clear() {
    impl()->clear();
  }

  size_t size() const {
    return impl()->size();
  }

  Expected<None> push(Var value) {
    return impl()->push(value);
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of sequence has the same sizeof()
  char _mem[sizeof(StdVector<int>)];

  inline const ISequence* impl() const {
    return reinterpret_cast<const ISequence*>(&_mem[0]);
  }

  inline ISequence* impl() {
    return reinterpret_cast<ISequence*>(&_mem[0]);
  }
};

}  // namespace er
