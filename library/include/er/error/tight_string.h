#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

namespace er {

/// immutable union object to tightly pack std::string and std::string_view together
/// the main idea to determine which one is in the memory is
/// compare sizes, because std::string(32 bytes) is bigger then std::string_view(16 bytes)
/// if last two words are zeroes TightString contains std::string_view
/// reduces dynamic allocations
struct TightString {
  TightString(const TightString& other) {
    if (other.is_owned()) {
      new (&_raw[0]) std::string(other.str());
    } else {
      zero_tail();
      view() = other.view();
    }
  }

  TightString& operator=(const TightString& other) {
    if (other.is_owned()) {
      new (&_raw[0]) std::string(other.str());
    } else {
      zero_tail();
      view() = other.view();
    }
    return *this;
  }

  TightString(TightString&& other) noexcept {
    if (other.is_owned()) {
      new (&_raw[0]) std::string(std::move(other.str()));
    } else {
      zero_tail();
      view() = other.view();
    }
  }

  TightString& operator=(TightString&& other) {
    if (other.is_owned()) {
      new (&_raw[0]) std::string(std::move(other.str()));
    } else {
      zero_tail();
      view() = other.view();
    }
    return *this;
  }

  TightString(const char* shared) {  // implicit
    zero_tail();
    view() = shared;
  }

  TightString(std::string_view shared) {  // implicit
    zero_tail();
    view() = shared;
  }

  TightString(std::string&& owned) {  // implicit
    new (&_raw[0]) std::string(std::move(owned));
  }

  TightString() {
    std::memset(&_raw[0], 0, sizeof(_raw));
  };

  ~TightString() {
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
    // has the correct behaviour in runtime
    if (is_owned()) {
      str().std::string::~string();
    }
  };

  [[nodiscard]] bool is_owned() const {
    return std::memcmp(&_raw[sizeof(std::string_view)], &kZeroes[0], kTailSize) != 0;
  }

  std::string_view get() const {
    if (is_owned()) {
      return str();
    }
    return view();
  }

 private:
  char _raw[sizeof(std::string)];

  inline std::string& str() noexcept {
    return *reinterpret_cast<std::string*>(&_raw[0]);
  }

  inline const std::string& str() const noexcept {
    return *reinterpret_cast<const std::string*>(&_raw[0]);
  }

  inline std::string_view& view() noexcept {
    return *reinterpret_cast<std::string_view*>(&_raw[0]);
  }

  inline const std::string_view& view() const noexcept {
    return *reinterpret_cast<const std::string_view*>(&_raw[0]);
  }

  static constexpr uint8_t kTailSize = sizeof(std::string) - sizeof(std::string_view);
  static constexpr char kZeroes[kTailSize]{};

  inline void zero_tail() {
    std::memset(&_raw[sizeof(std::string_view)], 0, kTailSize);
  }
};

}  // namespace er