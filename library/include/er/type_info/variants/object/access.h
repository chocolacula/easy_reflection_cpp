#pragma once

#include <cstdint>

namespace rr {

/// Represent common access modifiers kPublic, kProtected, kPrivate and kStatic
/// DO NOT edit names, they used as text by generator
enum class Access : uint8_t {  //
  kNone = 0,
  kPublic = 0b0001,
  kProtected = 0b0010,
  kPrivate = 0b0100,
  kStatic = 0b1000,
  kAll = 0b1111,
};

inline Access operator|(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline Access operator&(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace rr