#include <string_view>

#include "er/error/error.h"
#include "gtest/gtest.h"

using namespace rr;

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
