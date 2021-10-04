#pragma once

#include <cstdint>

namespace rr {

/// Represent common access modifiers kPublic, kProtected, kPrivate
/// kStatic could be combined with others modifiers
/// DO NOT edit names, they used as text by generator
enum class Access : uint8_t { kNone = 0, kPublic = 0b001, kProtected = 0b010, kPrivate = 0b011, kStatic = 0b100 };

inline Access operator|(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline Access operator&(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace rr