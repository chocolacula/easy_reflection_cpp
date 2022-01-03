#pragma once

#include <memory>

#include "c_array.h"
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
    reinterpret_cast<IArray*>(&_mem[0])->~IArray();
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IArray*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    return reinterpret_cast<IArray*>(&_mem[0])->unsafe_assign(ptr);
  }

  Var own_var() const {
    return reinterpret_cast<const IArray*>(&_mem[0])->own_var();
  }

  TypeId nested_type() const {
    return reinterpret_cast<const IArray*>(&_mem[0])->nested_type();
  }

  Expected<Var> at(size_t idx) {
    return reinterpret_cast<IArray*>(&_mem[0])->at(idx);
  }

  Expected<Var> operator[](size_t idx) {
    return reinterpret_cast<IArray*>(&_mem[0])->operator[](idx);
  }

  Expected<Var> front() {
    return reinterpret_cast<IArray*>(&_mem[0])->front();
  };

  Expected<Var> back() {
    return reinterpret_cast<IArray*>(&_mem[0])->back();
  };

  Expected<None> fill(Var filler) {
    return reinterpret_cast<IArray*>(&_mem[0])->fill(filler);
  }

  void for_each(std::function<void(Var)> callback) const {
    reinterpret_cast<const IArray*>(&_mem[0])->for_each(callback);
  }

  void for_each(std::function<void(Var)> callback) {
    reinterpret_cast<IArray*>(&_mem[0])->for_each(callback);
  }

  void unsafe_for_each(std::function<void(void*)> callback) const {
    reinterpret_cast<const IArray*>(&_mem[0])->unsafe_for_each(callback);
  }

  size_t size() const {
    return reinterpret_cast<const IArray*>(&_mem[0])->size();
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of array has the same sizeof()
  char _mem[sizeof(StdArray<int, 8>)];
};

}  // namespace er
