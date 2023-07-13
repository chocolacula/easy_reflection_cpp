#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "colors.h"

struct [[er::reflect]] Bicycle {
  Bicycle() = default;

  int id;
  bool is_hardtail = true;
  std::string manufacturer;
  std::string model;

  [[er::alias("weight")]] float frame_weight;

  [[er::alias("wheel_size")]] float wheel_size_inch;

  std::vector<Colors> colors;

  constexpr static inline bool kIsCool = true;

  std::string_view tune(Colors color, float wheel_size) {
    colors.push_back(color);
    wheel_size_inch = wheel_size;

    return "I am in fresh paint with new wheels";
  }

  [[er::alias("double")]]
  static int double_sum(int a, int b) {
    return (a + b) * 2;
  }
};
