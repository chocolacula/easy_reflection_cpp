#pragma once

#include <map>
#include <string_view>

#include "access.h"
#include "field_info.h"

namespace er {

class ComplexFieldIterator {
  using const_iterator = std::map<std::string_view, FieldDesc>::const_iterator;
  using item = std::pair<std::string_view, FieldInfo>;

 public:
  ComplexFieldIterator(const void* base, const_iterator it, const_iterator end, Access access)  //
      : _base(base), _it(it), _end(end), _access(access) {
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
    if ((_it->second.access() & _access) != Access::kNone) {
      return _base != nullptr || _it->second.is_static();
    }
    return false;
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
  Access _access;
};

}  // namespace er