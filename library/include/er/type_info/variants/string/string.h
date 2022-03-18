#pragma once

#include <cstddef>

#include "c_string.h"
#include "istring.h"
#include "std_basic_string.h"
#include "std_basic_string_view.h"

namespace er {

struct String final {
  String() = delete;

  template <typename T>
  String(std::basic_string<T>* str, bool is_const) {
    new (_mem) StdBasicString<T>(str, is_const);
  }

  template <typename T>
  String(std::basic_string_view<T>* str, bool is_const) {
    new (_mem) StdBasicStringView<T>(str);
  }

  template <typename T>
  String(const T** str, bool is_const) {
    new (_mem) CString<T>(str);
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IString*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    reinterpret_cast<IString*>(&_mem[0])->unsafe_assign(ptr);
  }

  std::string_view get() const {
    return reinterpret_cast<const IString*>(&_mem[0])->get();
  }

  Expected<None> set(std::string_view value) {
    return reinterpret_cast<IString*>(&_mem[0])->set(value);
  }

  Var var() const {
    return reinterpret_cast<const IString*>(&_mem[0])->var();
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of string has the same sizeof()
  char _mem[sizeof(CString<char>)];
};

}  // namespace er