#pragma once

#include <cstddef>

#include "field_iterator.h"

namespace er {

struct Fields {
  Fields(const void* base, const std::map<std::string_view, FieldDesc>* map, Access access, bool include_readonly)  //
      : _base(base), _map(map), _access(access), _include_readonly(include_readonly) {
  }

  size_t size() const {
    return _map->size();
  }

  FieldIterator begin() {
    return {_map, _base, _access, _include_readonly};
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
