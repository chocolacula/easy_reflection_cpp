#pragma once

#include <cstdint>

namespace er {

/// Represent common access modifiers kPublic, kProtected, kPrivate
/// DO NOT edit these three names, they used as text by generator project
/// and in addition kStatic and kReadOnly(for reference types like std::string_view).
/// They are organized like flags to make possible queries like
/// Object::get_fields(kProtected or kPrivate)
enum class FieldAttributes : uint8_t {  //
  kNone = 0,
  // ▼▼▼ do not edit ▼▼▼
  kPublic = 0b00001,
  kProtected = 0b00010,
  kPrivate = 0b00100,
  // ▲▲▲ do not edit ▲▲▲
  kStatic = 0b01000,
  kReadOnly = 0b10000,
  kAnyAccess = 0b00111,
  kAll = 0b11111,
};

inline FieldAttributes operator|(FieldAttributes lhs, FieldAttributes rhs) {
  return static_cast<FieldAttributes>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline FieldAttributes operator&(FieldAttributes lhs, FieldAttributes rhs) {
  return static_cast<FieldAttributes>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

}  // namespace er
