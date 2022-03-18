#pragma once

#include <cstdint>

namespace er {

/// Represent common access modifiers kPublic, kProtected, kPrivate
/// DO NOT edit names, they used as text by generator project
/// They are organized like flags to make possible queries like
/// Object::get_fields(kProtected or kPrivate)
enum class Access : uint8_t {  //
  kNone = 0,
  kPublic = 0b001,
  kProtected = 0b010,
  kPrivate = 0b100,
  kAll = 0b111,
};

inline Access operator|(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline Access operator&(Access lhs, Access rhs) {
  return static_cast<Access>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace er