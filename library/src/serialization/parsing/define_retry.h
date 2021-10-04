// #pragma once is deliberately missed

#include "er/tools/traits.h"

// return | macro similar to behavior of 'try' keyword but without 'catch'
//   try  | if some error occurred, it will be returned on upper level
// Note: don't forget to undef it
#define __retry(x)                                                                           \
  {                                                                                          \
    auto __ex___ = x;                                                                        \
    static_assert(is_expected_v<decltype(__ex___)>,                                          \
                  "__retry(x) macro is compatible only with Expected<T, ErrorT> structure"); \
    if (__ex___.is_error()) {                                                                \
      return __ex___.error();                                                                \
    }                                                                                        \
  }
//
