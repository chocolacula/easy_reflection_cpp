#pragma once

#include <memory>
#include <utility>

#include "c_array.h"
#include "er/type_info/variants/array/iarray.h"
#include "std_array.h"

namespace er {

struct Array final {
  Array() = delete;

  template <typename T, size_t size_v>
  Array(T (*array)[size_v], bool is_const) {
    new (_mem) CArray<T, size_v>(array, is_const);
  }

  template <typename T, size_t size_v>
  Array(std::array<T, size_v>* array, bool is_const) {
    new (_mem) StdArray<T, size_v>(array, is_const);
  }

  ~Array() {
    impl()->~IArray();
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

  Expected<Var> at(size_t idx) {
    return impl()->at(idx);
  }

  Expected<Var> operator[](size_t idx) {
    return impl()->operator[](idx);
  }

  Expected<Var> front() {
    return impl()->front();
  };

  Expected<Var> back() {
    return impl()->back();
  };

  Expected<None> fill(Var filler) {
    return impl()->fill(filler);
  }

  void for_each(std::function<void(Var)> callback) const {
    impl()->for_each(std::move(callback));
  }

  void for_each(std::function<void(Var)> callback) {
    impl()->for_each(std::move(callback));
  }

  void unsafe_for_each(std::function<void(void*)> callback) const {
    impl()->unsafe_for_each(std::move(callback));
  }

  size_t size() const {
    return impl()->size();
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of array has the same sizeof()
  char _mem[sizeof(StdArray<int, 0>)];

  inline const IArray* impl() const {
    return reinterpret_cast<const IArray*>(&_mem[0]);
  }

  inline IArray* impl() {
    return reinterpret_cast<IArray*>(&_mem[0]);
  }
};

}  // namespace er
