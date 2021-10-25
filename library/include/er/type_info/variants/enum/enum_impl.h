#pragma once

#include "er/constexpr_map.h"
#include "ienum.h"

namespace er {

template <typename T, size_t size_v>
struct EnumImpl final : public IEnum {
  EnumImpl() = delete;

  EnumImpl(T* value, bool is_const, const ConstexprMap<T, size_v>& all_constants)
      : _value(value), _is_const(is_const), _all_constants(all_constants) {
  }

  Var var() const override {
    return Var(_value, TypeId::get<T>(), _is_const);
  }

  std::string_view to_string() const override {
    return _all_constants.get_name(*_value).unwrap();
  }

  Expected<None> parse(std::string_view name) override {
    if (_is_const) {
      return Error("Cannot assign anything to const enum");
    }
    auto ex = _all_constants.get_value(name);

    return ex.template match(
        [this](T& v) -> Expected<None> {
          *_value = v;
          return None();
        },
        [](Error& err) -> Expected<None> { return err; });
  }

 private:
  T* _value;
  bool _is_const;
  const ConstexprMap<T, size_v>& _all_constants;
};

}  // namespace er
