#pragma once

#include <memory>

#include "enum_impl.h"

namespace er {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Enum final : IEnum {

  template <typename T, size_t size>
  Enum(T* value, bool is_const, const ConstexprMap<T, size>& all_constants)
      : _enum(std::make_shared<EnumImpl<T, size>>(value, is_const, all_constants)) {
  }

  Expected<None> assign(Var var) override {
    return _enum->assign(var);
  }

  void unsafe_assign(void* ptr) override {
    _enum->unsafe_assign(ptr);
  }

  Var var() const override {
    return _enum->var();
  }

  std::string_view to_string() const override {
    return _enum->to_string();
  }

  Expected<None> parse(std::string_view name) override {
    return _enum->parse(name);
  };

 private:
  std::shared_ptr<IEnum> _enum;
};

}  // namespace er