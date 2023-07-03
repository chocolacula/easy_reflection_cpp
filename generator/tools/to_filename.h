#pragma once

#include <cctype>
#include <string>

// returns filename in snake_case
inline std::string to_filename(const std::string& str) {
  std::string name;

  name += std::tolower(str[0]);
  for (auto i = 1; i < str.length(); i++) {
    char c = str[i];

    if (std::isupper(static_cast<unsigned char>(c)) != 0) {
      if (name.back() != '_') {
        name += '_';
      }
      name += std::tolower(c);
    } else {
      if (c == ':') {
        name += '_';
        i++;
      } else {
        name += c;
      }
    }
  }

  return name;
}