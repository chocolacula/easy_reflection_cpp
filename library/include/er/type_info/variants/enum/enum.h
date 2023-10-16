#pragma once

#include <memory>

#include "enum_impl.h"
#include "ienum.h"

namespace er {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Enum final {

  template <typename T, size_t size>
  Enum(T* value, bool is_const, const ConstexprMap<T, size>& all_constants) {
    new (_mem) EnumImpl<T, size>(value, is_const, all_constants);
  }

  Expected<None> assign(Var var) {
    return impl()->assign(var);
  }

  void unsafe_assign(void* ptr) {
    impl()->unsafe_assign(ptr);
  }

  Var var() const {
    return impl()->var();
  }

  std::string_view to_string() const {
    return impl()->to_string();
  }

  Expected<None> parse(std::string_view name) {
    return impl()->parse(name);
  };

 private:
  char _mem[sizeof(EnumImpl<int, 0>)];

  inline const IEnum* impl() const {
    return reinterpret_cast<const IEnum*>(&_mem[0]);
  }

  inline IEnum* impl() {
    return reinterpret_cast<IEnum*>(&_mem[0]);
  }
};

}  // namespace er
