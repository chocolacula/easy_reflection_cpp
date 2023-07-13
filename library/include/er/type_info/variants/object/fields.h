#pragma once

#include <cstddef>

#include "complex_field_iterator.h"

namespace er {

struct Fields {
  Fields(const void* base, const std::map<std::string_view, FieldDesc>* map, Access access, bool include_readonly)  //
      : _base(base), _map(map), _access(access), _include_readonly(include_readonly) {
  }

  size_t size() const {
    return _map->size();
  }

  ComplexFieldIterator begin() {
    ComplexFieldIterator it{_base, _map->begin(), _map->end(), _access, _include_readonly};
    if (!it.is_valid()) {
      it.next_valid();
    }
    return it;
  }

  // iterator type is not the same as begin
  std::map<std::string_view, FieldDesc>::const_iterator end() {
    return _map->end();
  }

 private:
  const void* _base;
  const std::map<std::string_view, FieldDesc>* _map;
  const Access _access;
  const bool _include_readonly;
};

}  // namespace er
