#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "nlohmann/json.hpp"

struct Context {
  std::string_view output_dir;

  // clangTooling run analysis for each input file
  // the files could intersect with each other
  // so a class/enum definition could be analysed multiple times
  // track them in this map
  std::unordered_map<std::string, nlohmann::json> result;
};
