#pragma once

#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <string>
#include <type_traits>

#include "append_buf.h"
#include "traits.h"

namespace rr {

template <typename T>
static typename std::enable_if_t<is_string_v<std::remove_reference_t<T>>, void>  //
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<std::is_same_v<std::remove_reference_t<T>, char*> ||
                                     std::is_same_v<std::remove_reference_t<T>, const char*>,
                                 void>  //
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<std::is_array_v<std::remove_reference_t<T>> &&  //
                                     std::is_same_v<array_value_t<T>, const char>,
                                 void>
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<std::is_integral_v<std::remove_reference_t<T>> &&         //
                                     !std::is_same_v<std::remove_reference_t<T>, bool> &&  //
                                     !std::is_same_v<std::remove_reference_t<T>, char>,
                                 void>
append(std::string* str, T&& arg) {
  *str += std::to_string(arg);
}

template <typename T>
static typename std::enable_if_t<std::is_floating_point_v<std::remove_reference_t<T>>, void>  //
append(std::string* str, T&& arg) {
  AppendBuf buf(str);
  std::ostream stream(&buf);
  stream << std::setiosflags(std::ios::fixed) << std::setprecision(2);
  stream << arg;
}

template <typename T>
static typename std::enable_if_t<std::is_same_v<std::remove_reference_t<T>, bool>, void>  //
append(std::string* str, T&& arg) {
  *str += arg ? "true" : "false";
}

template <typename T>
static typename std::enable_if_t<std::is_same_v<std::remove_reference_t<T>, char>, void>  //
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<std::is_same_v<std::remove_reference_t<T>, std::filesystem::path>, void>  //
append(std::string* str, T&& arg) {
  *str += arg.string();
}

static void format(std::string* result, std::string_view fmt, size_t i) {
  *result += fmt.substr(i, fmt.size() - i);
}

template <typename T, typename... Ts>
static void format(std::string* result, std::string_view fmt, size_t i, T&& arg, Ts&&... args) {
  auto pos = fmt.find('{', i);

  if (pos == std::string::npos) {
    *result += fmt.substr(i, fmt.size() - i);
    return;
  }
  *result += fmt.substr(i, pos - i);
  append(result, std::forward<T>(arg));
  format(result, fmt, pos + 2, std::forward<Ts>(args)...);
}

/// little format function with the single one purpose - format error messages with arguments
template <typename... Ts>
std::string format(std::string_view fmt, Ts&&... args) {
  std::string result;
  format(&result, fmt, 0, std::forward<Ts>(args)...);
  return result;
}

}  // namespace rr
