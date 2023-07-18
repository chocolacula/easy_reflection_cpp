#pragma once

#include <string>
#include <utility>

struct [[er::reflect]] Simple {
  Simple() : v1(0){};

  Simple(const Simple& other) {
    if (this == &other) {
      return;
    }
    v1 = other.v1;
    v2 = other.v2;
  }

  Simple& operator=(const Simple& other) noexcept {
    if (this == &other) {
      return *this;
    }
    v1 = other.v1;
    v2 = other.v2;

    return *this;
  }

  Simple(Simple&& other) noexcept : v1(other.v1), v2(std::move(other.v2)) {
  }

  Simple& operator=(Simple&& other) noexcept {
    v1 = other.v1;
    v2 = std::move(other.v2);

    return *this;
  }

  Simple(int integer, std::string str) : v1(integer), v2(str) {
  }

  int v1;
  std::string v2;

  bool operator<(const Simple& other) const {
    return v1 < other.v1;
  }

  bool operator==(const Simple& other) const {
    return (v1 == other.v1 && v2 == other.v2);
  }

  bool operator!=(const Simple& other) const {
    return !(*this == other);
  }
};
