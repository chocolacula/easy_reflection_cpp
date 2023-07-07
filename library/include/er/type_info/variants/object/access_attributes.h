#pragma once

#include <cstdint>

namespace er {

/// Represent common access modifiers kPublic, kProtected, kPrivate
/// DO NOT edit these three names, they used as text by generator project
enum class AccessModifiers : uint8_t {  //
  kPublic = 0b001,
  kProtected = 0b010,
  kPrivate = 0b100,
};

inline AccessModifiers operator|(AccessModifiers lhs, AccessModifiers rhs) {
  return static_cast<AccessModifiers>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline AccessModifiers operator&(AccessModifiers lhs, AccessModifiers rhs) {
  return static_cast<AccessModifiers>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace er
