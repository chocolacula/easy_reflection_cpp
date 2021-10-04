#pragma once

#include <cstring>
#include <string>
#include <string_view>

namespace rr {

/// union object to tightly pack std::string and std::string_view together
/// the main idea to determine which one is in the memory is
/// compare sizes, because std::string(32 bytes) is bigger then std::string_view(16 bytes)
/// size of both of them is a multiple of unsigned long(32 or 64 bits)
/// if last two words are zeroes TightString contains std::string_view
union TightString {

  TightString(const TightString& data) {
    if (data.is_owned()) {
      new (&owned) std::string(data.owned);
    } else {
      zero_tail();
      shared = data.shared;
    }
  }

  TightString& operator=(const TightString& data) {
    if (data.is_owned()) {
      new (&owned) std::string(data.owned);
    } else {
      zero_tail();
      shared = data.shared;
    }
    return *this;
  }

  TightString(TightString&& data) {
    if (data.is_owned()) {
      new (&owned) std::string(std::move(data.owned));
    } else {
      zero_tail();
      shared = data.shared;
    }
  }

  TightString& operator=(TightString&& data) {
    if (data.is_owned()) {
      new (&owned) std::string(std::move(data.owned));
    } else {
      zero_tail();
      shared = data.shared;
    }
    return *this;
  }

  TightString(const char* shared) {
    zero_tail();
    this->shared = shared;
  }

  TightString(std::string_view shared) {
    zero_tail();
    this->shared = shared;
  }

  TightString(std::string&& owned) {
    new (&this->owned) std::string(std::move(owned));
  }

  TightString() {
  };

  ~TightString() {
    if (is_owned()) {
      owned.~basic_string();
    }
  };

  [[nodiscard]] bool is_owned() const {
    return std::memcmp(&_raw[sizeof(std::string_view)], &kZeroes[0], kTailSize) != 0;
  }

  std::string_view shared;
  std::string owned;

 private:
  char _raw[sizeof(std::string)];

  static constexpr uint8_t kTailSize = sizeof(std::string) - sizeof(std::string_view);
  static constexpr char kZeroes[kTailSize]{};

  inline void zero_tail() {
    std::memset(&_raw[sizeof(std::string_view)], 0, kTailSize);
  }
};

}  // namespace rr