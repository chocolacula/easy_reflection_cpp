#pragma once

#include <cstddef>

#include "er/type_info/type_info.h"

namespace er {

struct UnknownActions {
  static TypeInfo reflect(void* /*value*/, bool /*is_const*/) {
    throw std::runtime_error("Cannot reflect a value with unknown type");
  }

  static std::string_view type_name() {
    return "unknown";
  }

  static size_t type_size() {
    return 0;
  }

  static void construct(void* /*p*/) {
    throw std::runtime_error("Cannot construct a value for unknown type");
  }

  static void destroy(void* /*p*/) {
    throw std::runtime_error("Cannot destroy a value with unknown type");
  }

  static void copy(void* /*to*/, const void* /*from*/) {
    throw std::runtime_error("Cannot copy a value with unknown type");
  }

  static void move(void* /*to*/, void* /*from*/) {
    throw std::runtime_error("Cannot move a value with unknown type");
  }

  // other types are self registering in The Great Table in the same place of a file
  // but HelperUnknown is NOT a self registering class
  // The Great Table put references to HelperUnknown methods itself
};

}  // namespace er
