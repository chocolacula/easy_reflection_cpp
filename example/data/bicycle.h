#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "colors.h"
#include "er/attributes.h"

ER_REFLECT()
struct Bicycle {
  Bicycle() = default;

  int id;
  bool is_hardtail = true;
  std::string manufacturer;
  std::string model;

  ER_ALIAS(weight)
  float frame_weight;

  ER_ALIAS(wheel_size)
  float wheel_size_inch;

  std::vector<Colors> colors;

  constexpr static inline bool kIsCool = true;

  std::string_view tune(Colors color, float wheel_size) {
    colors.push_back(color);
    wheel_size_inch = wheel_size;

    return "I am in fresh paint with new wheels";
  }
};
