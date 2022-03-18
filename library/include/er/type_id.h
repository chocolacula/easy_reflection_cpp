#pragma once

#include <array>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "er/tools/traits.h"

#ifndef NDEBUG
namespace er::reflection {
std::string_view type_name(uint32_t id);
}  // namespace er::reflection
#endif

namespace er {

/// copyable value type with single one int under the hood
/// represent sequential type id
struct TypeId {
  TypeId() : _id(0) {
  }

  /// proxy function for using with implicit type in TypeId::get<T>() way without argument
  template <typename T>
  static TypeId get() {
    return get(static_cast<T*>(nullptr));
  }

  /// the main function of TypeId mechanism
  template <typename T>
  static typename std::enable_if_t<is_class_v<T>, TypeId>  //
  get(T* ptr) {
    return TypeId(0);
  }

  template <typename T>
  static typename std::enable_if_t<std::is_enum_v<T>, TypeId>  //
  get(T* ptr) {
    return TypeId(0);
  }

  template <typename T>
  static typename std::enable_if_t<std::is_integral_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<is_string_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T, size_t size>
  static TypeId get(T (*array)[size]);

  template <typename T, size_t size>
  static TypeId get(std::array<T, size>* array);

  template <typename T>
  static typename std::enable_if_t<is_sequence_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<is_map_v<T>, TypeId>  //
  get(T* ptr);

  // other methods
  bool operator==(const TypeId& other) const {
    return _id == other._id;
  }

  bool operator!=(const TypeId& other) const {
    return _id != other._id;
  }

  uint32_t number() const {
    return _id;
  }

 private:
  uint32_t _id;

#ifndef NDEBUG
  std::string type_name = "unknown";
#endif

  explicit TypeId(uint32_t id) : _id(id) {
#ifndef NDEBUG
    type_name = reflection::type_name(_id);
#endif
  }
};

}  // namespace er