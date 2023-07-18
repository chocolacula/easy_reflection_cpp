#pragma once

#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>

struct [[er::reflect]] Tresholds {
  uint8_t u8_min = 0;
  uint16_t u16_min = 0;
  uint32_t u32_min = 0;
  uint64_t u64_min = 0;

  uint8_t u8_max = UCHAR_MAX;
  uint16_t u16_max = USHRT_MAX;
  uint32_t u32_max = UINT_MAX;
  uint64_t u64_max = ULONG_MAX;

  int8_t i8_min = SCHAR_MIN;
  int8_t i8_max = SCHAR_MAX;
  int16_t i16_min = SHRT_MIN;
  int16_t i16_max = SHRT_MAX;
  int32_t i32_min = INT_MIN;
  int32_t i32_max = INT_MAX;
  int64_t i64_min = LONG_MIN;
  int64_t i64_max = LONG_MAX;

  float f32_min = -FLT_MAX;
  float f32_max = FLT_MAX;
  double f64_min = -DBL_MAX;
  double f64_max = DBL_MAX;

  float f32_ninf = -std::numeric_limits<float>::infinity();
  float f32_inf = std::numeric_limits<float>::infinity();
  float f32_nan = std::nanf("");

  double f64_ninf = -std::numeric_limits<double>::infinity();
  double f64_inf = std::numeric_limits<double>::infinity();
  double f64_nan = std::nan("");
};
