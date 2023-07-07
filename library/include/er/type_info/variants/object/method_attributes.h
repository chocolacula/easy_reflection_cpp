#pragma once

#include <cstdint>

namespace er {

/// Represent common access modifiers kPublic, kProtected, kPrivate
/// DO NOT edit these three names, they used as text by generator project
/// and in addition kStatic. They are organized like flags to make possible queries like
/// Object::get_methods(kProtected or kPrivate)
enum class MethodAttributes : uint8_t {  //
  kNone = 0,
  // ▼▼▼ do not edit ▼▼▼
  kPublic = 0b00001,
  kProtected = 0b00010,
  kPrivate = 0b00100,
  // ▲▲▲ do not edit ▲▲▲
  kStatic = 0b01000,
  kAnyAccess = 0b00111,
  kAll = 0b01111,
};

inline MethodAttributes operator|(MethodAttributes lhs, MethodAttributes rhs) {
  return static_cast<MethodAttributes>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline MethodAttributes operator&(MethodAttributes lhs, MethodAttributes rhs) {
  return static_cast<MethodAttributes>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace er
