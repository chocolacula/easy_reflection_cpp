#pragma once

#include <cstddef>

#include "complex_method_iterator.h"

namespace er {

struct Methods {
  Methods(void* base, const std::map<std::string_view, MethodDesc>* map, MethodAttributes access)  //
      : _base(base), _map(map), _access(access) {
  }

  size_t size() const {
    return _map->size();
  }

  ComplexMethodIterator begin() {
    ComplexMethodIterator it{_base, _map->begin(), _map->end(), _access};
    if (!it.is_valid()) {
      it.next_valid();
    }
    return it;
  }

  // iterator type is not the same as begin
  std::map<std::string_view, MethodDesc>::const_iterator end() {
    return _map->end();
  }

 private:
  void* _base;
  const std::map<std::string_view, MethodDesc>* _map;
  MethodAttributes _access;
};

}  // namespace er
