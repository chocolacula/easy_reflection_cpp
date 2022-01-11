#pragma once

#include <map>
#include <string>
#include <vector>

#include "er/attributes.h"

ER_REFLECT()
struct ComplexMapObj {
  struct Key {
    int v1;
    int v2;

    bool operator<(const Key& other) const {
      return v1 < other.v1 && v2 < other.v2;
    }
  };

  std::map<Key, std::string> content = {
      {{1, 1}, "string one"},  //
      {{2, 2}, "string two"}   //
  };
};

ER_REFLECT()
struct ComplexMapMap {
  std::map<std::map<int, int>, std::string> content = {
      {{{1, 1}}, "string one"},  //
      {{{2, 2}}, "string two"}   //
  };
};

ER_REFLECT()
struct ComplexMapVec {
  std::map<std::vector<int>, std::string> content = {
      {{1, 1}, "string one"},  //
      {{2, 2}, "string two"}   //
  };
};