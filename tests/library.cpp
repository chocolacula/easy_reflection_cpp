#include <string_view>

// #include "er/error/error.h"
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
