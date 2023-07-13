#pragma once

#include <cstddef>
#include <map>
#include <string_view>

#include "field_info.h"

namespace er {

class ComplexFieldIterator {
  using const_iterator = std::map<std::string_view, FieldDesc>::const_iterator;
  using item = std::pair<std::string_view, FieldInfo>;

 public:
  ComplexFieldIterator(const void* base, const_iterator it, const_iterator end, Access acc, bool include_readonly)  //
      : _base(base), _it(it), _end(end), _acc(acc), _include_readonly(include_readonly) {
  }

  ComplexFieldIterator& operator++() noexcept {
    next_valid();
    return *this;
  };

  ComplexFieldIterator operator++(int) noexcept {
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

  item operator*() const noexcept {
    return std::make_pair(_it->first, FieldInfo(_base, &_it->second));
  };

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

 private:
  const void* _base;
  const_iterator _it;
  const const_iterator _end;
  const Access _acc;
  const bool _include_readonly;

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
