#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../random/random.h"
#include "er/attributes.h"
#include "pointers.h"

ER_REFLECT()
enum class Numbers { kOne, kTwo, kThree };

ER_REFLECT()
struct Various {
  Various() = default;

  Various(const Various& other) {
    ch = other.ch;
    uch = other.uch;
    sch = other.sch;

    u8 = other.u8;
    u16 = other.u16;
    u32 = other.u32;
    u64 = other.u64;

    i8 = other.i8;
    i16 = other.i16;
    i32 = other.i32;
    i64 = other.i64;

    f32 = other.f32;
    f64 = other.f64;

    std::memcpy(&arr[0], &other.arr[0], sizeof(arr));

    str = other.str;

    std_arr = other.std_arr;
    vec = other.vec;
    list = other.list;
    deq = other.deq;
    que = other.que;
    stack = other.stack;
    set = other.set;
    un_set = other.un_set;

    map = other.map;
    un_map = other.un_map;

    obj = other.obj;
    ptrs = other.ptrs;

    monstro = other.monstro;
  }

  Various& operator=(const Various& other) {
    if (this == &other) {
      return *this;
    }

    ch = other.ch;
    uch = other.uch;
    sch = other.sch;

    u8 = other.u8;
    u16 = other.u16;
    u32 = other.u32;
    u64 = other.u64;

    i8 = other.i8;
    i16 = other.i16;
    i32 = other.i32;
    i64 = other.i64;

    f32 = other.f32;
    f64 = other.f64;

    std::memcpy(&arr[0], &other.arr[0], sizeof(arr));

    str = other.str;

    std_arr = other.std_arr;
    vec = other.vec;
    list = other.list;
    deq = other.deq;
    que = other.que;
    stack = other.stack;
    set = other.set;
    un_set = other.un_set;

    map = other.map;
    un_map = other.un_map;

    obj = other.obj;
    ptrs = other.ptrs;

    monstro = other.monstro;

    return *this;
  }

  char ch;
  unsigned char uch;
  signed char sch;

  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;

  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;

  float f32;
  double f64;

  int arr[3];

  std::string str;

  // string_view and C string are referencing to const data
  std::string_view str_v = "c++ 17 string_view";
  const char* c_str = "good old C string";

  std::array<int, 3> std_arr;
  std::vector<int> vec;
  std::list<int> list;
  std::deque<int> deq;
  std::queue<int> que;
  std::stack<int> stack;
  std::set<int> set;
  std::unordered_set<int> un_set;

  std::map<int, Numbers> map;
  std::unordered_map<int, std::string> un_map;

  // check an empty sequence and a map
  std::vector<int> empty_vec;
  std::map<int, int> empty_map;

  Simple obj;
  Pointers ptrs;

  std::vector<std::map<Simple, std::list<std::set<int>>>> monstro;

  static Various make_random() {
    Various v;

    v.ch = Random::get_int<char>('a', 'z');
    v.uch = Random::get_int<unsigned char>('a', 'z');
    v.sch = Random::get_int<signed char>('a', 'z');

    v.u8 = Random::get_int<uint8_t>();
    v.u16 = Random::get_int<uint16_t>();
    v.u32 = Random::get_int<uint32_t>();
    v.u64 = Random::get_int<uint64_t>();

    v.i8 = Random::get_int<int8_t>();
    v.i16 = Random::get_int<int16_t>();
    v.i32 = Random::get_int<int32_t>();
    v.i64 = Random::get_int<int64_t>();

    v.f32 = Random::get_float(0.0, 10.0);
    v.f64 = Random::get_float<double>(0.0, 10.0);

    v.arr[0] = Random::get_int();
    v.arr[1] = Random::get_int();
    v.arr[2] = Random::get_int();

    v.str = generate_string(128);

    v.std_arr = {Random::get_int(), Random::get_int(), Random::get_int()};
    v.vec = {Random::get_int(), Random::get_int(), Random::get_int()};
    v.list = {Random::get_int(), Random::get_int(), Random::get_int()};
    v.deq = {Random::get_int(), Random::get_int(), Random::get_int()};

    v.que.push(Random::get_int());
    v.que.push(Random::get_int());
    v.que.push(Random::get_int());

    v.stack.push(Random::get_int());
    v.stack.push(Random::get_int());
    v.stack.push(Random::get_int());

    v.set = {Random::get_int(), Random::get_int(), Random::get_int()};
    v.un_set = {Random::get_int(), Random::get_int(), Random::get_int()};

    v.map = {{Random::get_int(), Numbers::kOne},
             {Random::get_int(), Numbers::kTwo},
             {Random::get_int(), Numbers::kThree}};

    v.un_map = {{Random::get_int(), generate_string(32)},
                {Random::get_int(), generate_string(32)},
                {Random::get_int(), generate_string(32)}};

    v.obj = {Random::get_int(), generate_string(64)};
    v.ptrs = Pointers::make_random();

    v.monstro = {{
        {Simple(Random::get_int(), generate_string(64)),
         {{Random::get_int(), Random::get_int(), Random::get_int()},
          {Random::get_int(), Random::get_int(), Random::get_int()}}},
        {Simple(Random::get_int(), generate_string(64)),
         {{Random::get_int(), Random::get_int(), Random::get_int()},
          {Random::get_int(), Random::get_int(), Random::get_int()}}}  //
    }};

    return v;
  }

  static Various make_default() {
    Various v;

    v.ch = 'f';
    v.uch = 'O';
    v.sch = 'O';

    v.u8 = 7;
    v.u16 = 1024;
    v.u32 = 1000000;
    v.u64 = 1000000000;

    v.i8 = -7;
    v.i16 = -1000;
    v.i32 = -1000000;
    v.i64 = -1000000000;

    v.f32 = 3.14;
    v.f64 = -9.9999;

    v.arr[0] = 1;
    v.arr[1] = 2;
    v.arr[2] = 3;

    v.str = "common stl string";

    v.std_arr = {1, 2, 3};
    v.vec = {1, 2, 3};
    v.list = {1, 2, 3};
    v.deq = {1, 2, 3};

    v.que.push(1);
    v.que.push(2);
    v.que.push(3);

    v.stack.push(1);
    v.stack.push(2);
    v.stack.push(3);

    v.set = {1, 2, 3};
    v.un_set = {1, 2, 3};

    v.map = {{1, Numbers::kOne}, {2, Numbers::kTwo}, {3, Numbers::kThree}};
    v.un_map = {{1, "one"}, {2, "two"}, {3, "three"}};

    v.obj = {888, "nested object"};
    v.ptrs = Pointers::make_default();

    v.monstro = {{
        {Simple(222, "some string"), {{-1, 2, 5}, {5, 2, -1}}},
        {Simple(333, "the second string"), {{38, 16, 87}, {44, 41, 38}}}  //
    }};

    return v;
  }
};
