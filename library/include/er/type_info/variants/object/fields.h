#pragma once

#include "field_iterator.h"

namespace rr {

struct Fields {
  explicit Fields(const std::map<std::string_view, FieldInfo>* map, Access access)  //
      : _map(map), _access(access) {
  }

  FieldIterator begin() {
    return FieldIterator(_map->begin(), _map->end(), _access);
  }

  FieldIterator end() {
    return FieldIterator(_map->end(), _map->end(), _access);
  }

 private:
  const std::map<std::string_view, FieldInfo>* _map;
  Access _access;
};

}  // namespace rr
