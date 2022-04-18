#include "er/serialization/binary.h"
#include "er/serialization/json.h"
#include "er/serialization/simd_json.h"
#include "er/serialization/yaml.h"
#include "er/tools/format.h"
#include "generated/reflection.h"
#include "gtest/gtest.h"

using namespace er;

void compare_tresholds(const Tresholds& lhs, const Tresholds& rhs) {
  ASSERT_TRUE(lhs.u8_min == rhs.u8_min &&    //
              lhs.u16_min == rhs.u16_min &&  //
              lhs.u32_min == rhs.u32_min &&  //
              lhs.u64_min == rhs.u64_min);

  ASSERT_TRUE(lhs.u8_max == rhs.u8_max &&    //
              lhs.u16_max == rhs.u16_max &&  //
              lhs.u32_max == rhs.u32_max &&  //
              lhs.u64_max == rhs.u64_max);

  ASSERT_TRUE(lhs.i8_min == rhs.i8_min &&    //
              lhs.i16_min == rhs.i16_min &&  //
              lhs.i32_min == rhs.i32_min &&  //
              lhs.i64_min == rhs.i64_min);

  ASSERT_TRUE(lhs.i8_max == rhs.i8_max &&    //
              lhs.i16_max == rhs.i16_max &&  //
              lhs.i32_max == rhs.i32_max &&  //
              lhs.i64_max == rhs.i64_max);

  ASSERT_TRUE(std::abs(lhs.f32_min - rhs.f32_min) < 0.1 &&  //
              std::abs(lhs.f32_max - rhs.f32_max) < 0.1 &&  //
              std::abs(lhs.f64_min - rhs.f64_min) < 0.1 &&  //
              std::abs(lhs.f64_max - rhs.f64_max) < 0.1);

  ASSERT_TRUE(lhs.f32_ninf == rhs.f32_ninf &&  //
              lhs.f32_inf == rhs.f32_inf &&    //
              std::isnan(lhs.f32_nan) &&       //
              std::isnan(rhs.f32_nan));

  ASSERT_TRUE(lhs.f64_ninf == rhs.f64_ninf &&  //
              lhs.f64_inf == rhs.f64_inf &&    //
              std::isnan(lhs.f64_nan) &&       //
              std::isnan(rhs.f64_nan));
}

TEST(SerializationJson, Thresholds) {
  Tresholds struct1;

  auto str = er::serialization::json::to_string(&struct1).unwrap();
  auto struct2 = er::serialization::json::from_string<Tresholds>(str).unwrap();

  compare_tresholds(struct1, struct2);
}

#ifdef USE_SIMD_JSON
TEST(SerializationSimdJson, Thresholds) {
  Tresholds struct1;

  auto str = er::serialization::json::to_string(&struct1).unwrap();
  auto struct2 = er::serialization::simd_json::from_string<Tresholds>(str).unwrap();

  compare_tresholds(struct1, struct2);
}
#endif

TEST(SerializationYaml, Thresholds) {
  Tresholds struct1;

  auto str = er::serialization::yaml::to_string(&struct1).unwrap();
  auto struct2 = er::serialization::yaml::from_string<Tresholds>(str).unwrap();

  compare_tresholds(struct1, struct2);
}

TEST(SerializationBinary, Thresholds) {
  Tresholds struct1;

  auto binary_data = er::serialization::binary::to_vector(&struct1).unwrap();
  auto struct2 = er::serialization::binary::from_vector<Tresholds>(binary_data).unwrap();

  compare_tresholds(struct1, struct2);
}
