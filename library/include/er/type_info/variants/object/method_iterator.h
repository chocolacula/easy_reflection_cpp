#pragma once

#include <cstddef>
#include <map>
#include <string_view>

#include "method_info.h"

namespace er {

/// Skip iterator for methods collection, steps over items with unwanted properties
class MethodIterator {
  using const_iterator = std::map<std::string_view, MethodDesc>::const_iterator;
  using item = std::pair<std::string_view, MethodInfo>;

 public:
  MethodIterator(const std::map<std::string_view, MethodDesc>* map,  //
                 const void* base,                                   //
                 Access acc)
      : _it(map->begin()),  //
        _end(map->cend()),  //
        _base(base),        //
        _acc(acc) {
    // start from a valid element
    if (!is_valid()) {
      next_valid();
    }
  }

  MethodIterator& operator++() noexcept {
    next_valid();
    return *this;
  };

  MethodIterator operator++(int) noexcept {
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
    return std::make_pair(_it->first, MethodInfo(_base, &_it->second));
  };

 private:
  const_iterator _it;
  const const_iterator _end;
  const void* _base;
  const Access _acc;

  bool is_valid() {
    return right_access() && (_base != nullptr || _it->second.is_static());
  }

  void next_valid() {
    do {
      ++_it;
    } while (!is_valid() && _it != _end);
  }

  inline bool right_access() {
    return (_it->second.access() & _acc & (Access::kPublic | Access::kProtected | Access::kPrivate)) != Access::kNone;
  }
};

}  // namespace er
