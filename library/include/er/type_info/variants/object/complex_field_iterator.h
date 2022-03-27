#pragma once

#include <cstddef>
#include <map>
#include <string_view>

#include "field_attributes.h"
#include "field_info.h"

namespace er {

class ComplexFieldIterator {
  using const_iterator = std::map<std::string_view, FieldDesc>::const_iterator;
  using item = std::pair<std::string_view, FieldInfo>;

 public:
  ComplexFieldIterator(const void* base, const_iterator it, const_iterator end, FieldAttributes atr)  //
      : _base(base), _it(it), _end(end), _atr(atr) {
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
    return !(!right_access() ||           //
             !right_static_readonly() ||  //
             (_base == nullptr && !_it->second.is_static()));
  }

  void next_valid() {
    do {
      ++_it;
    } while (!is_valid() && _it != _end);
  }

 private:
  const void* _base;
  const_iterator _it;
  const_iterator _end;
  FieldAttributes _atr;

  inline bool right_access() {
    return (_it->second.attributes() & _atr & FieldAttributes::kAnyAccess) != FieldAttributes::kNone;
  }

  // build from Karnaugh Map
  inline bool right_static_readonly() {
    bool nx0 = (_it->second.attributes() & FieldAttributes::kReadOnly) == FieldAttributes::kNone;
    bool nx1 = (_it->second.attributes() & FieldAttributes::kStatic) == FieldAttributes::kNone;
    bool x2 = (_atr & FieldAttributes::kReadOnly) != FieldAttributes::kNone;
    bool x3 = (_atr & FieldAttributes::kStatic) != FieldAttributes::kNone;

    return (nx0 || x2) && (nx1 || x3);
  }
};

}  // namespace er