#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "er/serialization/binary.h"
#include "er/serialization/json.h"
#include "er/serialization/yaml.h"
#include "er/tools/format.h"
#include "generated/reflection.h"
#include "gtest/gtest.h"

using namespace er;

void compare_tresolds(const Tresholds& lhs, const Tresholds& rhs) {
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
}

void compare_various(const Various& lhs, const Various& rhs) {
  ASSERT_TRUE(lhs.ch == rhs.ch &&    //
              lhs.uch == rhs.uch &&  //
              lhs.sch == rhs.sch);

  ASSERT_TRUE(lhs.u8 == rhs.u8 &&    //
              lhs.u16 == rhs.u16 &&  //
              lhs.u32 == rhs.u32 &&  //
              lhs.u64 == rhs.u64);

  ASSERT_TRUE(lhs.i8 == rhs.i8 &&    //
              lhs.i16 == rhs.i16 &&  //
              lhs.i32 == rhs.i32 &&  //
              lhs.i64 == rhs.i64);

  ASSERT_TRUE(std::abs(lhs.f32 - rhs.f32) < 0.1 &&  //
              std::abs(lhs.f64 - rhs.f64) < 0.1);   //

  auto n = sizeof(lhs.arr) / sizeof(lhs.arr[0]);
  for (auto i = 0; i < n; i++) {
    ASSERT_EQ(lhs.arr[i], rhs.arr[i]);
  }

  ASSERT_EQ(lhs.str, rhs.str);
  ASSERT_EQ(lhs.std_arr, rhs.std_arr);
  ASSERT_EQ(lhs.vec, rhs.vec);
  ASSERT_EQ(lhs.list, rhs.list);
  ASSERT_EQ(lhs.deq, rhs.deq);
  ASSERT_EQ(lhs.que, rhs.que);
  ASSERT_EQ(lhs.stack, rhs.stack);
  ASSERT_EQ(lhs.set, rhs.set);
  ASSERT_EQ(lhs.un_set, rhs.un_set);
  ASSERT_EQ(lhs.map, rhs.map);
  ASSERT_EQ(lhs.un_map, rhs.un_map);

  ASSERT_EQ(lhs.monstro, rhs.monstro);
}

TEST(SerializationJson, Thresholds) {
  Tresholds struct1;

  auto str = er::serialization::json::to_string(&struct1).unwrap();
  auto struct2 = er::serialization::json::from_string<Tresholds>(str).unwrap();

  compare_tresolds(struct1, struct2);
}

TEST(SerializationYaml, Thresholds) {
  Tresholds struct1;

  auto str = er::serialization::yaml::to_string(&struct1).unwrap();
  auto struct2 = er::serialization::yaml::from_string<Tresholds>(str).unwrap();

  compare_tresolds(struct1, struct2);
}

TEST(SerializationBinary, Thresholds) {
  Tresholds struct1;

  auto binary_data = er::serialization::binary::to_vector(&struct1).unwrap();
  auto struct2 = er::serialization::binary::from_vector<Tresholds>(binary_data).unwrap();

  compare_tresolds(struct1, struct2);
}

TEST(SerializationJson, BackAndForth) {
  auto struct1 = Various::make_default();

  auto time_1 = std::chrono::steady_clock::now();

  auto str = er::serialization::json::to_string(&struct1).unwrap();

  auto time_2 = std::chrono::steady_clock::now();

  auto struct2 = er::serialization::json::from_string<Various>(str).unwrap();

  auto time_3 = std::chrono::steady_clock::now();

  auto forth = std::chrono::duration_cast<std::chrono::microseconds>(time_2 - time_1).count();
  auto back = std::chrono::duration_cast<std::chrono::microseconds>(time_3 - time_2).count();
  std::cout << er::format(
      "JSON serialization {} us, "
      "deserialization {} us, generated {} bytes\n",
      forth, back, str.size());

  compare_various(struct1, struct2);

  std::stringstream stream;
  er::serialization::json::to_stream(stream, &struct1);
  auto struct3 = er::serialization::json::from_stream<Various>(stream).unwrap();

  compare_various(struct1, struct3);
}

TEST(SerializationYaml, BackAndForth) {
  auto struct1 = Various::make_default();

  auto time_1 = std::chrono::steady_clock::now();

  auto str = er::serialization::yaml::to_string(&struct1).unwrap();

  auto time_2 = std::chrono::steady_clock::now();

  auto struct2 = er::serialization::yaml::from_string<Various>(str).unwrap();

  auto time_3 = std::chrono::steady_clock::now();

  auto forth = std::chrono::duration_cast<std::chrono::microseconds>(time_2 - time_1).count();
  auto back = std::chrono::duration_cast<std::chrono::microseconds>(time_3 - time_2).count();
  std::cout << er::format(
      "YAML serialization {} us, "
      "deserialization {} us, generated {} bytes\n",
      forth, back, str.size());

  compare_various(struct1, struct2);

  std::stringstream stream;
  er::serialization::yaml::to_stream(stream, &struct1);
  auto struct3 = er::serialization::yaml::from_stream<Various>(stream).unwrap();

  compare_various(struct1, struct3);
}

TEST(SerializationYaml, Anchors) {
  auto struct1 = Various::make_default();

  std::ifstream input;
  input.open(std::string(PROJECT_ROOT) + "/tests/data/various_anchors.yaml");

  std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  auto struct2 = er::serialization::yaml::from_string<Various>(str).unwrap();

  compare_various(struct1, struct2);
}

TEST(SerializationBinary, BackAndForth) {
  auto struct1 = Various::make_default();

  auto time_1 = std::chrono::steady_clock::now();

  auto binary_data = er::serialization::binary::to_vector(&struct1).unwrap();

  auto time_2 = std::chrono::steady_clock::now();

  auto struct2 = er::serialization::binary::from_vector<Various>(binary_data).unwrap();

  auto time_3 = std::chrono::steady_clock::now();

  auto forth = std::chrono::duration_cast<std::chrono::microseconds>(time_2 - time_1).count();
  auto back = std::chrono::duration_cast<std::chrono::microseconds>(time_3 - time_2).count();
  std::cout << er::format(
      "Binary serialization {} us, "
      "deserialization {} us, generated {} bytes\n",
      forth, back, binary_data.size());

  compare_various(struct1, struct2);

  std::stringstream stream;
  er::serialization::binary::to_stream(stream, &struct1);
  auto struct3 = er::serialization::binary::from_stream<Various>(stream).unwrap();

  compare_various(struct1, struct3);
}