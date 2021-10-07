#pragma once

#include <istream>

#include "er/expected.h"
#include "er/reflection/reflection.h"

namespace rr::serialization {

struct json {  // NOLINT namespace like name
  template <typename T>
  static Expected<T> from_string(std::string_view str) {
    T obj;

    auto info = reflection::reflect(&obj);
    auto exp = deserialize(str, &info);
    if (exp.is_error()) {
      return exp.error();
    }
    return obj;
  }

  template <typename T>
  static Expected<T> from_stream(std::istream& stream) {
    T obj;

    auto info = reflection::reflect(&obj);
    auto exp = deserialize(stream, &info);
    if (exp.is_error()) {
      return exp.error();
    }
    return obj;
  }

  template <typename T>
  static Expected<std::string> to_string(const T& obj) {
    auto info = reflection::reflect(obj);

    std::string result;
    serialize(info, &result);
    return result;
  }

 private:
  static void serialize(const TypeInfo& info, std::string* result);
  static Expected<None> deserialize(std::string_view str, TypeInfo* info);
  static Expected<None> deserialize(std::istream& stream, TypeInfo* info);
};

}  // namespace rr::serialization