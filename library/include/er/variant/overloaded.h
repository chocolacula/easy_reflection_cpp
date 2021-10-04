#pragma once

namespace rr {

// a little helper for std::variant
template <typename... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};

// explicit deduction guide (not needed as of C++20)
template <typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

} // namespace rr