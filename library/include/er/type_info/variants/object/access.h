#pragma once

#include <cstdint>

namespace er {

/// Represent common access modifiers kPublic, kProtected, kPrivate, etc.
/// DO NOT edit they used as text by generator project
enum class Access : uint8_t {  //
  // clang-format off
  kNone      = 0b00000,
  kPublic    = 0b00001,
  kProtected = 0b00010,
  kPrivate   = 0b00100,
  kStatic    = 0b01000,
  kConst     = 0b10000,
  kAll       = 0b11111,
  // clang-format on
};

inline Access operator|(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline Access operator&(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace er
