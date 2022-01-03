#include <string_view>

#include "data/various.h"
#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/map/std_unordered_map.h"
#include "er/variable/box.h"
#include "generated/reflection.h"
#include "gtest/gtest.h"

using namespace er;

TEST(TightString, Sizeof) {
  ASSERT_TRUE(sizeof(std::string) > sizeof(std::string_view));
  ASSERT_EQ(sizeof(TightString), sizeof(Error));
  ASSERT_EQ(sizeof(TightString), sizeof(std::string));
}

TEST(TightString, Ownership) {
  for (auto i = 0; i < 1000; i++) {
    TightString str;
    str = "Constant string which we can easily share";
    ASSERT_FALSE(str.is_owned());
  }

  for (auto i = 0; i < 1000; i++) {
    TightString str;
    str = std::string_view("String view with sharable constant string inside");
    ASSERT_FALSE(str.is_owned());
  }

  for (auto i = 0; i < 1000; i++) {
    TightString str;
    str = std::string("std::string which own the data in dynamic memory");
    ASSERT_TRUE(str.is_owned());
  }
}

TEST(Array, Sizeof) {
  auto arr = sizeof(Array);

  ASSERT_EQ(arr, sizeof(StdArray<int, 8>));
  ASSERT_EQ(arr, sizeof(CArray<int, 8>));
}

TEST(Sequence, Sizeof) {
  auto seq = sizeof(Sequence);

  ASSERT_EQ(seq, sizeof(StdVector<int>));
  ASSERT_EQ(seq, sizeof(StdList<int>));
  ASSERT_EQ(seq, sizeof(StdDeque<int>));
  ASSERT_EQ(seq, sizeof(StdStack<int>));
  ASSERT_EQ(seq, sizeof(StdQueue<int>));
  ASSERT_EQ(seq, sizeof(StdStack<int>));
  ASSERT_EQ(seq, sizeof(StdSet<int>));
  ASSERT_EQ(seq, sizeof(StdUnorderedSet<int>));
}

TEST(Map, Sizeof) {
  auto map = sizeof(Map);

  ASSERT_EQ(map, sizeof(StdMap<int, int>));
  ASSERT_EQ(map, sizeof(StdUnorderedMap<int, int>));
}

TEST(Box, Allocation) {
  // keep it in separate code block to check for correct deletion
  {
    // heap allocated type
    auto type = TypeId::get<Big>();
    Box box(type);

    auto* ptr = box.var().raw_mut();
    ASSERT_NE(ptr, nullptr);
    ASSERT_TRUE(box.uses_heap());
  }
  {
    // a regular one type
    auto type = TypeId::get<uint64_t>();
    Box box(type);

    auto* ptr = box.var().raw_mut();
    ASSERT_NE(ptr, nullptr);
    ASSERT_FALSE(box.uses_heap());
  }
  {
    // the biggest type which could be allocated on the stack
    auto type = TypeId::get<std::unordered_map<int, int>>();
    Box box(type);

    auto* ptr = box.var().raw_mut();
    ASSERT_NE(ptr, nullptr);
    ASSERT_FALSE(box.uses_heap());
  }
}

TEST(TypeInfo, VariantIndex) {
  bool b;
  auto info = er::reflection::reflect(&b);
  ASSERT_TRUE(info.is<Bool>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kBool);

  int i;
  info = er::reflection::reflect(&i);
  ASSERT_TRUE(info.is<Integer>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kInteger);

  float f;
  info = er::reflection::reflect(&f);
  ASSERT_TRUE(info.is<Floating>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kFloating);

  std::string str;
  info = er::reflection::reflect(&str);
  ASSERT_TRUE(info.is<String>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kString);

  Numbers e;
  info = er::reflection::reflect(&e);
  ASSERT_TRUE(info.is<Enum>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kEnum);

  Various::ComplexValue o;
  info = er::reflection::reflect(&o);
  ASSERT_TRUE(info.is<Object>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kObject);

  int a[4];
  info = er::reflection::reflect(&a);
  ASSERT_TRUE(info.is<Array>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kArray);

  std::vector<int> seq;
  info = er::reflection::reflect(&seq);
  ASSERT_TRUE(info.is<Sequence>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kSequence);

  std::unordered_map<int, int> m;
  info = er::reflection::reflect(&m);
  ASSERT_TRUE(info.is<Map>());
  ASSERT_EQ(info.get_kind(), TypeInfo::Kind::kMap);
}
