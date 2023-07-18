#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../print.h"
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

  size_t set_color(const std::vector<Colors>& new_colors) {
    colors = new_colors;
    println("The bike has been refinished");

    return colors.size();
  }
};
