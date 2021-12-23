#pragma once

#include <stdexcept>
#include <type_traits>
#include <variant>

#include "overloaded.h"

namespace er {

// declaring a slightly more readable state of nothing
using None = std::monostate;

template <typename... T>
struct Variant {

  template <typename SomeT, typename = std::enable_if_t<!std::is_same_v<SomeT, Variant>, void>>
  Variant(const SomeT& value) : _content(value) {  // NOLINT implicit constructor
  }

  template <typename SomeT, typename = std::enable_if_t<!std::is_same_v<SomeT, Variant>, void>>
  Variant(SomeT&& value) : _content(std::move(value)) {  // NOLINT implicit constructor
  }

  template <typename... FuncT>
  decltype(auto) match(FuncT... functions) {
    return std::visit(Overloaded{functions...}, _content);
  }

  template <typename... FuncT>
  decltype(auto) match_move(FuncT... functions) {
    return std::visit(Overloaded{functions...}, std::move(_content));
  }

  template <typename... FuncT>
  decltype(auto) match(FuncT... functions) const {
    return std::visit(Overloaded{functions...}, _content);
  }

  template <typename SomeT>
  [[nodiscard]] inline bool is() const {
    return std::holds_alternative<SomeT>(_content);
  }

  [[nodiscard]] inline size_t variant_idx() const {
    return _content.index();
  }

  template <typename SomeT>
  SomeT& get() {
    auto pointer = std::get_if<SomeT>(&_content);

    if (pointer == nullptr) {
      throw std::runtime_error("The instance of requested type doesn't exist");
    }
    return *pointer;
  }

  template <typename SomeT>
  inline const SomeT& unsafe_get() const {
    return std::get<SomeT>(_content);
  }

  template <typename SomeT>
  inline SomeT& unsafe_get() {
    return std::get<SomeT>(_content);
  }

 protected:
  std::variant<T...> _content;
};

}  // namespace er