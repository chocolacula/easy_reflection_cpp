#pragma once

#include "tight_string.h"

namespace er {

struct Error {
  explicit Error(const char* message) : _data(message) {
  }

  explicit Error(std::string_view message) : _data(message) {
  }

  explicit Error(std::string&& message) : _data(std::move(message)) {
  }

  Error(const Error& other) = default;

  Error(Error&& other) = default;

  Error& operator=(const Error& other) {
    if (this == &other) {
      return *this;
    }

    _data = other._data;

    return *this;
  }

  Error& operator=(Error&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    _data = std::move(other._data);

    return *this;
  }

  ~Error() = default;

  /// what() uses similar with plain exceptions interface
  /// returns what happened
  std::string_view what() const {
    return _data.get();
  }

  friend std::ostream& operator<<(std::ostream& os, const Error& err) {
    os << err.what();
    return os;
  }

 private:
  TightString _data;
};

}  // namespace er
