#pragma once

#include <stdexcept>
#include <variant>

#include "error/error.h"
#include "variant/variant.h"

namespace rr {

#define BASE Variant<T, ErrorT>

template <typename T, typename ErrorT = Error>
struct Expected : public BASE {

  template <typename = std::enable_if_t<!std::is_same_v<T, Expected>, void>>
  Expected(const T& value) : BASE(value) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<T, Expected>, void>>
  Expected(T&& value) : BASE(std::move(value)) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<ErrorT, Expected>, void>>
  Expected(const ErrorT& error) : BASE(error) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<ErrorT, Expected>, void>>
  Expected(ErrorT&& error) : BASE(std::move(error)) {  // NOLINT implicit constructor
  }

  bool is_error() {
    return BASE::template is<ErrorT>();
  }

  ErrorT error() {
    return std::get<ErrorT>(std::move(BASE::_content));
  }

  T unwrap() {
    if (is_error()) {
      throw std::runtime_error(error().what().data());
    }
    return std::get<T>(std::move(BASE::_content));
  }
};

#undef BASE

}  // namespace rr