#pragma once

#include <memory>

#include "enum_impl.h"
#include "er/type_info/variants/enum/ienum.h"

namespace er {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Enum final {

  template <typename T, size_t size>
  Enum(T* value, bool is_const, const ConstexprMap<T, size>& all_constants) {
    new (_mem) EnumImpl<T, size>(value, is_const, all_constants);
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IEnum*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    reinterpret_cast<IEnum*>(&_mem[0])->unsafe_assign(ptr);
  }

  Var var() const {
    return reinterpret_cast<const IEnum*>(&_mem[0])->var();
  }

  std::string_view to_string() const {
    return reinterpret_cast<const IEnum*>(&_mem[0])->to_string();
  }

  Expected<None> parse(std::string_view name) {
    return reinterpret_cast<IEnum*>(&_mem[0])->parse(name);
  };

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of array has the same sizeof()
  char _mem[sizeof(EnumImpl<int, 0>)];
};

}  // namespace er