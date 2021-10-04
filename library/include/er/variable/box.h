#pragma once

#include "var.h"

namespace rr {

struct Box {
  Box(const Box& other) = delete;
  Box(Box&& other) = default;

  explicit Box(TypeId id);
  ~Box();

  Var var();
  Box clone();

 private:
  TypeId _type;
  bool _optimized;

  // max size of stack memory for dynamic allocation optimization
  static const size_t kMemSize = sizeof(std::unordered_map<int, int>);
  union Data {
    void* ptr;
    char stack_mem[kMemSize];
  } _data;
};

}  // namespace rr
