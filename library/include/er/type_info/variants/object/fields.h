#pragma once

#include <cstddef>

#include "field_iterator.h"

namespace er {

struct Fields {
  explicit Fields(const std::map<std::string_view, FieldInfo>* map, Access access)  //
      : _map(map), _access(access) {
  }

  size_t size() const {
    return _map->size();
  }

  FieldIterator begin() {
    return {_map->begin(), _map->end(), _access};
  }

  FieldIterator end() {
    return {_map->end(), _map->end(), _access};
  }

 private:
  const std::map<std::string_view, FieldInfo>* _map;
  Access _access;
};

}  // namespace er
