#pragma once

#include <array>
#include <map>
#include <memory>

#include "../random/random.h"
#include "../tools.h"
#include "simple.h"

struct [[er::reflect]] Pointers {
  Pointers() = default;

  Pointers(const Pointers& other)
      : obj(other.obj),            //
        null_ptr(other.null_ptr),  //
        arr(other.arr),            //
        vec(other.vec),            //
        map(other.map) {
  }

  Pointers& operator=(const Pointers& other) noexcept {
    if (this == &other) {
      return *this;
    }

    obj = other.obj;
    null_ptr = other.null_ptr;
    arr = other.arr;
    vec = other.vec;
    map = other.map;

    return *this;
  }

  bool operator==(const Pointers& other) const {
    if (*obj != *other.obj) {
      return false;
    }
    if (null_ptr != other.null_ptr) {
      return false;
    }
    if (arr.size() != other.arr.size()) {
      return false;
    }
    for (auto i = 0; i < arr.size(); ++i) {
      if (*arr[i] != *other.arr[i]) {
        return false;
      }
    }
    if (vec.size() != other.vec.size()) {
      return false;
    }
    for (auto i = 0; i < vec.size(); ++i) {
      if (*vec[i] != *other.vec[i]) {
        return false;
      }
    }
    if (map.size() != other.map.size()) {
      return false;
    }
    auto m = map.begin();
    while (m != map.end()) {
      auto om = other.map.find(m->first);
      if (om == other.map.end()) {
        return false;
      }
      if (m->first != om->first || *m->second != *om->second) {
        return false;
      }
      m++;
    }
    return true;
  }

  bool operator!=(const Pointers& other) const {
    return !(*this == other);
  }

  std::shared_ptr<Simple> obj;
  std::shared_ptr<Simple> null_ptr;
  std::array<std::shared_ptr<Simple>, 3> arr;
  std::vector<std::shared_ptr<Simple>> vec;
  std::map<int, std::shared_ptr<Simple>> map;

  static Pointers make_random() {
    Pointers p;

    p.obj = std::make_shared<Simple>(Random::get_int(), generate_string(64));

    p.arr[0] = std::make_shared<Simple>(Random::get_int(), generate_string(64));
    p.arr[1] = std::make_shared<Simple>(Random::get_int(), generate_string(64));
    p.arr[2] = std::make_shared<Simple>(Random::get_int(), generate_string(64));

    p.vec = {std::make_shared<Simple>(Random::get_int(), generate_string(64)),
             std::make_shared<Simple>(Random::get_int(), generate_string(64)),
             std::make_shared<Simple>(Random::get_int(), generate_string(64))};

    p.map = {{Random::get_int(), std::make_shared<Simple>(Random::get_int(), generate_string(64))},
             {Random::get_int(), std::make_shared<Simple>(Random::get_int(), generate_string(64))},
             {Random::get_int(), std::make_shared<Simple>(Random::get_int(), generate_string(64))}};
    return p;
  }

  static Pointers make_default() {
    Pointers p;

    p.obj = std::make_shared<Simple>(999, "nested object behind shared_ptr");

    p.arr[0] = std::make_shared<Simple>(111, "an object behind shared_ptr in an array at 0");
    p.arr[1] = std::make_shared<Simple>(222, "an object behind shared_ptr in an array at 1");
    p.arr[2] = std::make_shared<Simple>(333, "an object behind shared_ptr in an array at 2");

    p.vec = {std::make_shared<Simple>(111, "an object behind shared_ptr in a vector at 0"),
             std::make_shared<Simple>(222, "an object behind shared_ptr in a vector at 1"),
             std::make_shared<Simple>(333, "an object behind shared_ptr in a vector at 2")};

    p.map = {{11, std::make_shared<Simple>(111, "an object behind shared_ptr in a map at 0")},
             {22, std::make_shared<Simple>(222, "an object behind shared_ptr in a map at 1")},
             {33, std::make_shared<Simple>(333, "an object behind shared_ptr in a map at 2")}};
    return p;
  }
};
