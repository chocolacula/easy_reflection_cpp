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
  ComplexMethodIterator(const void* base, const_iterator it, const_iterator end, Access acc)  //
      : _base(base), _it(it), _end(end), _acc(acc) {
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
    return right_access() && (_base != nullptr || _it->second.is_static());
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

  inline bool right_access() {
    return (_it->second.access() & _acc & (Access::kPublic | Access::kProtected | Access::kPrivate)) != Access::kNone;
  }
};

}  // namespace er
