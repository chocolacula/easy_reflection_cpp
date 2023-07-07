#pragma once

#include <cstddef>
#include <map>
#include <string_view>

#include "method_info.h"

namespace er {

class ComplexMethodIterator {
  using const_iterator = std::map<std::string_view, MethodDesc>::const_iterator;
  using item = std::pair<std::string_view, MethodInfo>;

 public:
  ComplexMethodIterator(void* base, const_iterator it, const_iterator end, MethodAttributes atr)  //
      : _base(base), _it(it), _end(end), _atr(atr) {
  }

  ComplexMethodIterator& operator++() noexcept {
    next_valid();
    return *this;
  };

  ComplexMethodIterator operator++(int) noexcept {
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
    return std::make_pair(_it->first, MethodInfo(_base, &_it->second));
  };

  bool is_valid() {
    return right_access() &&  //
                              //  right_static_readonly() &&  //
           (_base != nullptr || _it->second.is_static());
  }

  void next_valid() {
    do {
      ++_it;
    } while (!is_valid() && _it != _end);
  }

 private:
  void* _base;
  const_iterator _it;
  const_iterator _end;
  MethodAttributes _atr;

  inline bool right_access() {
    return (_it->second.attributes() & _atr & MethodAttributes::kAnyAccess) != MethodAttributes::kNone;
  }

  // built from Karnaugh Map
  // inline bool right_static_readonly() {
  //   bool nx0 = (_it->second.attributes() & MethodAttributes::kReadOnly) == MethodAttributes::kNone;
  //   bool nx1 = (_it->second.attributes() & MethodAttributes::kStatic) == MethodAttributes::kNone;
  //   bool x2 = (_atr & MethodAttributes::kReadOnly) != MethodAttributes::kNone;
  //   bool x3 = (_atr & MethodAttributes::kStatic) != MethodAttributes::kNone;

  //   return (nx0 || x2) && (nx1 || x3);
  // }
};

}  // namespace er
