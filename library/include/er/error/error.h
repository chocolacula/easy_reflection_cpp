#pragma once

#include "tight_string.h"

namespace rr {

struct Error {
  explicit Error(const char* message) : _data(message) {
  }

  explicit Error(std::string_view message) : _data(message) {
  }

  explicit Error(std::string&& message) : _data(std::move(message)) {
  }

  Error(const Error& other) noexcept : _data(other._data) {
  }

  Error& operator=(const Error& other) {
    if (this == &other) {
      return *this;
    }

    _data = other._data;

    return *this;
  }

  Error(Error&& other) noexcept : _data(std::move(other._data)) {
  }

  Error& operator=(Error&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    _data = std::move(other._data);

    return *this;
  }

  /// what() uses similar with plain exceptions interface
  /// returns what happened
  std::string_view what() const {

    if (_data.is_owned()) {
      return _data.owned;
    }

    return _data.shared;
  }

  friend std::ostream& operator<<(std::ostream& os, const Error& err) {
    os << err.what();
    return os;
  }

 protected:
  TightString _data;
};

}  // namespace rr
