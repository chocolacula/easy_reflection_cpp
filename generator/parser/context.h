#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "nlohmann/json.hpp"

struct AttrReflect {
  bool withNonPublic = false;
  bool withBase = false;
};

struct Context {
  std::unordered_map<unsigned, AttrReflect> reflect_map;
  std::unordered_map<unsigned, std::string> alias_map;
  std::unordered_set<unsigned> excludes;

  std::string_view output_dir;

  // clangTooling run analysis for each input file
  // the files could intersect with each other
  // so a class/enum definition could be analysed multiple times
  // track them in this map
  std::unordered_map<std::string, nlohmann::json> result;
};