#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "er/attributes.h"

ER_REFLECT()
enum class Numbers { kOne, kTwo, kThree };

ER_REFLECT()
struct Various {
  char ch = 'f';
  unsigned char uch = 'O';
  signed char sch = 'O';

  uint8_t u8 = 7;
  uint16_t u16 = 1024;
  uint32_t u32 = 1000000;
  uint64_t u64 = 1000000000;

  int8_t i8 = -7;
  int16_t i16 = -1000;
  int32_t i32 = -1000000;
  int64_t i64 = -1000000000;

  float f32 = 3.14;
  double f64 = -9.9999;

  int arr[3] = {1, 2, 3};

  std::string str = "common stl string";
  // std::string_view str_v = "c++ 17 string view";

  std::array<int, 3> std_arr = {1, 2, 3};
  std::vector<int> vec = {1, 2, 3};
  std::list<int> list = {1, 2, 3};
  std::deque<int> deq = {1, 2, 3};
  std::queue<int> que{{1, 2, 3}};
  std::stack<int> stack{{1, 2, 3}};
  std::set<int> set = {1, 2, 3};
  std::unordered_set<int> un_set = {1, 2, 3};

  std::map<int, Numbers> map = {{1, Numbers::kOne}, {2, Numbers::kTwo}, {3, Numbers::kThree}};
  std::unordered_map<int, std::string> un_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  struct ComplexValue {
    ComplexValue() = default;
    ComplexValue(int integer, std::string str) : v1(integer), v2(std::move(str)) {
    }

    int v1;
    std::string v2;

    bool operator<(const ComplexValue& other) const {
      return v1 < other.v1 || v2 < other.v2;
    }

    bool operator==(const ComplexValue& other) const {
      return v1 == other.v1 && v2 == other.v2;
    }
  };

  ComplexValue obj = {888, "nested object"};

  std::vector<std::map<ComplexValue, std::list<std::set<int>>>> monstro = {{
      {ComplexValue(222, "some string"), {{-1, 2, 5}, {5, 2, -1}}},
      {ComplexValue(333, "the second string"), {{38, 16, 87}, {44, 41, 38}}}  //
  }};
};
