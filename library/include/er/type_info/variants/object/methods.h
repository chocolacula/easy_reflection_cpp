#pragma once

#include <cstddef>

#include "method_iterator.h"

namespace er {

struct Methods {
  Methods(void* base, const std::map<std::string_view, MethodDesc>* map, Access acc)  //
      : _base(base), _map(map), _acc(acc) {
  }

  size_t size() const {
    return _map->size();
  }

  MethodIterator begin() {
    return {_map, _base, _acc};
  }

  // iterator type is not the same as begin
  std::map<std::string_view, MethodDesc>::const_iterator end() {
    return _map->end();
  }

 private:
  const void* _base;
  const std::map<std::string_view, MethodDesc>* _map;
  const Access _acc;
};

}  // namespace er
