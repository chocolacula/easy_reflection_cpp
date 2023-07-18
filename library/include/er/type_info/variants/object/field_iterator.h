#pragma once

#include <cstddef>
#include <map>
#include <string_view>

#include "field_info.h"

namespace er {

/// Skip iterator for fields collection, steps over items with unwanted properties
class FieldIterator {
  using const_iterator = std::map<std::string_view, FieldDesc>::const_iterator;

 public:
  FieldIterator(const std::map<std::string_view, FieldDesc>* map,  //
                const void* base,                                  //
                Access acc,                                        //
                bool include_readonly)
      : _it(map->begin()),  //
        _end(map->cend()),  //
        _base(base),        //
        _acc(acc),          //
        _include_readonly(include_readonly) {
    // start from a valid element
    if (!is_valid()) {
      next_valid();
    }
  }

  FieldIterator& operator++() noexcept {
    next_valid();
    return *this;
  };

  FieldIterator operator++(int) noexcept {
    auto t = *this;
    ++(*this);
    return t;
  };

  bool operator==(const const_iterator& other) const noexcept {
    return _it != other;
  };

  bool operator!=(const const_iterator& other) const noexcept {
    return _it != other;
  };

  auto operator*() const noexcept {
    return std::make_pair(_it->first, FieldInfo(_base, &_it->second));
  };

 private:
  const_iterator _it;
  const const_iterator _end;
  const void* _base;
  const Access _acc;
  const bool _include_readonly;

  bool is_valid() {
    return right_access() &&           //
           right_static_readonly() &&  //
           (_base != nullptr || _it->second.is_static());
  }

  void next_valid() {
    do {
      ++_it;
    } while (!is_valid() && _it != _end);
  }

  inline bool right_access() {
    return (_it->second.access() & _acc & (Access::kPublic | Access::kProtected | Access::kPrivate)) != Access::kNone;
  }

  // built from Karnaugh Map
  inline bool right_static_readonly() {
    bool nx0 = !_it->second.is_readonly();
    bool nx1 = (_it->second.access() & Access::kStatic) == Access::kNone;
    bool x2 = _include_readonly;
    bool x3 = (_acc & Access::kStatic) != Access::kNone;

    return (nx0 || x2) && (nx1 || x3);
  }
};

}  // namespace er
