#pragma once

#include <stdint.h>

#include <limits>
#include <random>
#include <string>
#include <string_view>

class Random {
 public:
  enum class CharSet {
    Numbers = 0b000001,
    Symbols = 0b000010,
    English = 0b000100,
    Chinese = 0b001000,
    Cyrillic = 0b010000,
    Emojis = 0b100000,
    WoSymbols = Numbers | English | Chinese | Cyrillic | Emojis,
    All = WoSymbols | Symbols,
  };

  template <typename T>
  static void randomise_array(T* array, size_t length) {
    for (auto i = 0; i < length; i++) {
      array[i] = get_int<T>();
    }
  }

  static std::string get_rand_string(size_t length, CharSet chars);

  template <typename T = int>
  static T get_int();

  template <typename T = int>
  static T get_int(T from, T to);

  template <typename T = float>
  static T get_float() {
    std::uniform_real_distribution<T> dist(-std::numeric_limits<T>::max(),  //
                                           std::numeric_limits<T>::max());
    return dist(_mt);
  }

  template <typename T = float>
  static T get_float(T from, T to) {
    std::uniform_real_distribution<T> dist(from, to);
    return dist(_mt);
  }

 private:
  static std::random_device _rd;
  static std::mt19937 _mt;

  static constexpr inline std::string_view kNumbers = "0123456789";

  // it could break a syntax, especial yaml
  static constexpr inline std::string_view kSymbols = " -_+=~/|\\()[]{}<>!?&*@#%$^\"'`.,;:";

  static constexpr inline std::string_view kEnglish = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  static constexpr inline std::string_view kChinese =
      u8"電買車紅無東馬風時鳥語頭魚園長島愛紙書見假佛德拜黑冰兔妒每壤步巢惠鞋莓圓聽實證龍賣龜藝"
      u8"戰繩關鐵圖團轉廣惡豐腦雜壓雞價樂氣廳發勞劍歲權燒贊兩譯觀營處齒驛櫻產藥讀畫顏聲學體點麥"
      u8"蟲舊會萬盜寶國醫雙晝觸來黃區";

  static constexpr inline std::string_view kCyrillic =
      u8"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

  static constexpr inline std::string_view kEmojis =
      u8"😂❤️😍🤣😊😩⌚️🙏😭🔥😘👍🥰😎😆🎸😁😉🤔🥁😅😔🎧🙄😜♻️"
      u8"😒👌😢💪✅🍔👏😇🌹❎✌🚲❄Ⓜ️🐝☘️💢⚒🍀✏️☔️🚙🚀🚤♂♀⛄";
};

inline Random::CharSet operator|(Random::CharSet lhs, Random::CharSet rhs) {
  return static_cast<Random::CharSet>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline Random::CharSet operator&(Random::CharSet lhs, Random::CharSet rhs) {
  return static_cast<Random::CharSet>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

#if _MSC_VER
// error C2338: static_assert failed:
// 'note: char, signed char, unsigned char, char8_t, int8_t, and uint8_t are not allowed'
#define __char_t uint16_t
#else
#define __char_t uint8_t
#endif

template <typename T>
inline T Random::get_int() {
  if constexpr (std::is_same_v<T, char> ||           //
                std::is_same_v<T, unsigned char> ||  //
                std::is_same_v<T, signed char>) {
    std::uniform_int_distribution<__char_t> dist(static_cast<__char_t>(std::numeric_limits<T>::min()),  //
                                                 static_cast<__char_t>(std::numeric_limits<T>::max()));
    return static_cast<T>(dist(_mt));
  } else {
    std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(),  //
                                          std::numeric_limits<T>::max());
    return dist(_mt);
  }
}

template <typename T>
inline T Random::get_int(T from, T to) {
  if constexpr (std::is_same_v<T, char> ||           //
                std::is_same_v<T, unsigned char> ||  //
                std::is_same_v<T, signed char>) {
    std::uniform_int_distribution<__char_t> dist(static_cast<__char_t>(from),  //
                                                 static_cast<__char_t>(to));
    return static_cast<T>(dist(_mt));
  } else {
    std::uniform_int_distribution<T> dist(from, to);
    return dist(_mt);
  }
}

#undef __char_t
