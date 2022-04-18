#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "er/serialization/binary.h"
#include "er/serialization/json.h"
#include "er/serialization/simd_json.h"
#include "er/serialization/yaml.h"
#include "er/tools/format.h"
#include "generated/reflection.h"
#include "gtest/gtest.h"

using namespace er;

const int kN = 10000;

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
  ASSERT_EQ(lhs.str_v, rhs.str_v);
  ASSERT_TRUE(std::strcmp(lhs.c_str, rhs.c_str) == 0);
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

  ASSERT_EQ(lhs.obj, rhs.obj);

  auto vl = lhs.monstro.begin();
  auto vr = rhs.monstro.begin();
  while (vl != lhs.monstro.end() && vr != rhs.monstro.end()) {
    ASSERT_EQ(vl->size(), vr->size());

    auto ml = vl->begin();
    while (ml != vl->end()) {
      auto mr = vr->find(ml->first);

      ASSERT_EQ(ml->first, mr->first);
      ASSERT_EQ(ml->second, mr->second);
      ml++;
    }

    vl++;
    vr++;
  }
}

TEST(SerializationJson, BackAndForth) {
  for (auto i = 0; i < kN; i++) {
    auto struct1 = Various::make_random();

    auto str = er::serialization::json::to_string(&struct1).unwrap();
    auto struct2 = er::serialization::json::from_string<Various>(str).unwrap();

    compare_various(struct1, struct2);

    std::stringstream stream;
    er::serialization::json::to_stream(stream, &struct1);
    auto struct3 = er::serialization::json::from_stream<Various>(stream).unwrap();

    compare_various(struct1, struct3);
  }
}

#ifdef USE_SIMD_JSON
TEST(SerializationSimdJson, BackAndForth) {
  for (auto i = 0; i < kN; i++) {
    auto struct1 = Various::make_random();

    auto str = er::serialization::json::to_string(&struct1).unwrap();
    auto struct2 = er::serialization::simd_json::from_string<Various>(str).unwrap();

    compare_various(struct1, struct2);
  }
}
#endif

TEST(SerializationYaml, BackAndForth) {
  for (auto i = 0; i < kN; i++) {
    auto struct1 = Various::make_random();

    auto str = er::serialization::yaml::to_string(&struct1).unwrap();
    auto struct2 = er::serialization::yaml::from_string<Various>(str).unwrap();

    compare_various(struct1, struct2);

    std::stringstream stream;
    er::serialization::yaml::to_stream(stream, &struct1);
    auto struct3 = er::serialization::yaml::from_stream<Various>(stream).unwrap();

    compare_various(struct1, struct3);
  }
}

TEST(SerializationYaml, Anchors) {
  auto struct1 = Various::make_default();

  std::ifstream input;
  input.open(std::string(PROJECT_ROOT) + "/data/yaml/various_anchors.yaml");

  std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  input.close();

  auto struct2 = er::serialization::yaml::from_string<Various>(str).unwrap();

  compare_various(struct1, struct2);
}

TEST(SerializationBinary, BackAndForth) {
  for (auto i = 0; i < kN; i++) {
    auto struct1 = Various::make_random();

    auto binary_data = er::serialization::binary::to_vector(&struct1).unwrap();
    auto struct2 = er::serialization::binary::from_vector<Various>(binary_data).unwrap();

    compare_various(struct1, struct2);

    std::stringstream stream;
    er::serialization::binary::to_stream(stream, &struct1);
    auto struct3 = er::serialization::binary::from_stream<Various>(stream).unwrap();

    compare_various(struct1, struct3);
  }
}