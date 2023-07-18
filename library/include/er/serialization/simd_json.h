#pragma once

#include <istream>

#include "er/expected.h"
#include "er/variable/var.h"

namespace er::serialization {

#ifdef USE_SIMD_JSON

struct simd_json {  // NOLINT namespace like name
  template <typename T>
  static Expected<T> from_string(std::string_view str) {
    T obj;

    auto exp = deserialize(Var(&obj), str);
    if (exp.is_error()) {
      return exp.error();
    }
    return obj;
  }

 private:
  static Expected<None> deserialize(Var var, std::string_view str);
};

#endif

}  // namespace er::serialization
