#pragma once

#include <istream>

#include "er/expected.h"
#include "er/variable/var.h"

namespace er::serialization {

struct json {  // NOLINT namespace like name
  template <typename T>
  static Expected<T> from_string(std::string_view str) {
    T obj;

    auto exp = deserialize(Var(&obj), str);
    if (exp.is_error()) {
      return exp.error();
    }
    return obj;
  }

  template <typename T>
  static Expected<T> from_stream(std::istream& stream) {
    T obj;

    auto exp = deserialize(Var(&obj), stream);
    if (exp.is_error()) {
      return exp.error();
    }
    return obj;
  }

  template <typename T>
  static Expected<std::string> to_string(const T* obj) {
    std::string result;
    serialize(&result, Var(obj));
    return result;
  }

  template <typename T>
  static void to_stream(std::ostream& stream, const T* obj) {
    serialize(stream, Var(obj));
  }

 private:
  static void serialize(std::string* str, Var var);
  static void serialize(std::ostream& stream, Var var);
  static Expected<None> deserialize(Var var, std::string_view str);
  static Expected<None> deserialize(Var var, std::istream& stream);
};

}  // namespace er::serialization