#include "random.h"

#include <cstddef>
#include <stdexcept>
#include <string_view>

std::random_device Random::_rd;
std::mt19937 Random::_mt(Random::_rd());

std::string_view char_at(size_t i, std::string_view str) {
  uint8_t n = str[i];
  while ((n >> 6) == 0b10) {
    n = str[--i];
  }

  auto len = 1;

  if ((n & 0xE0) == 0xC0) {
    len = 2;
  } else if ((n & 0xF0) == 0xE0) {
    len = 3;
  } else if ((n & 0xF8) == 0xF0) {
    len = 4;
  }

  return str.substr(i, len);
}

std::string Random::get_rand_string(size_t length, CharSet chars) {
  std::string result;

  const CharSet kSETS[] = {
      CharSet::Numbers, CharSet::Symbols, CharSet::English, CharSet::Chinese, CharSet::Cyrillic, CharSet::Emojis,
  };

  int variants[6];
  for (auto i = 0, j = 0; i < 6; j = (j + 1) % 6) {
    if ((chars & kSETS[j]) == kSETS[j]) {
      variants[i++] = j;
    }
  }

  for (auto i = 0; i < length; i++) {
    switch (variants[get_int(0, 6 - 1)]) {
      case 0:
        result += char_at(get_int<size_t>(0, kNumbers.length() - 1), kNumbers);
        break;
      case 1:
        result += char_at(get_int<size_t>(0, kSymbols.length() - 1), kSymbols);
        break;
      case 2:
        result += char_at(get_int<size_t>(0, kEnglish.length() - 1), kEnglish);
        break;
      case 3:
        result += char_at(get_int<size_t>(0, kChinese.length() - 1), kChinese);
        break;
      case 4:
        result += char_at(get_int<size_t>(0, kCyrillic.length() - 1), kCyrillic);
        break;
      case 5:
        result += char_at(get_int<size_t>(0, kEmojis.length() - 1), kEmojis);
        break;
      default:
        throw std::runtime_error("Unknown CharSet in random string generation");
        break;
    }
  }

  return result;
}
