#pragma once

#include <map>
#include <string_view>

#include "access.h"
#include "field_info.h"

namespace er {

class FieldIterator {
  using const_iterator = std::map<std::string_view, FieldInfo>::const_iterator;
  using item = std::map<std::string_view, FieldInfo>::value_type;

 public:
  FieldIterator(const_iterator it, const_iterator end, Access access)  //
      : _it(it), _end(end), _access(access) {
  }

  FieldIterator& operator++() noexcept {
    // if _access == Access::Static (only)
    // all static fields are valid no matter public or private
#pragma unroll
    do {
      ++_it;
    } while (_it != _end && !_it->second.is_const() && (_it->second.access() & _access) == Access::kNone);
    return *this;
  };

  FieldIterator operator++(int) noexcept {
    auto t = *this;
    ++(*this);
    return t;
  };

  bool operator!=(const FieldIterator& other) const noexcept {
    return _it != other._it;
  };

  const item& operator*() const noexcept {
    return *_it;
  };

 private:
  const_iterator _it;
  const_iterator _end;
  Access _access;
};

}  // namespace er