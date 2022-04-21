#pragma once

#include <string>

#include "random/random.h"

static std::string generate_string(size_t length) {
  auto str = Random::get_rand_string(length, Random::CharSet::WoSymbols);

  // YAML string should not start and end with ' '
  // if (str[0] == ' ') {
  // str[0] = Random::get_int<char>('a', 'z');
  // }
  // if (str[str.length() - 1] == ' ') {
  // str[str.length() - 1] = Random::get_int<char>('a', 'z');
  // }

  return str;
}