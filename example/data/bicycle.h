#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "color.h"
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
};
