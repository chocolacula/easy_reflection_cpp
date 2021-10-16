#pragma once

#include <cstddef>
#include <istream>

#include "er/expected.h"
#include "er/variable/var.h"

namespace rr::serialization {

struct binary {  // NOLINT namespace like name
  template <typename T>
  static Expected<T> from_vector(const std::vector<uint8_t>& vector) {
    T obj;

    auto exp = deserialize(Var(&obj), vector);
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
  static Expected<std::vector<uint8_t>> to_vector(const T* obj) {
    std::vector<uint8_t> result;
    serialize(&result, Var(obj));
    return result;
  }

  template <typename T>
  static void to_stream(std::ostream& stream, const T* obj) {
    serialize(stream, Var(obj));
  }

  static void serialize(std::vector<uint8_t>* vector, Var var);
  static void serialize(std::ostream& stream, Var var);
  static Expected<None> deserialize(Var var, const std::vector<uint8_t>& vector);
  static Expected<None> deserialize(Var var, std::istream& stream);
};

}  // namespace rr::serialization