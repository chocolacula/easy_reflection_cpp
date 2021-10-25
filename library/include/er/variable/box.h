#pragma once

#include "var.h"

namespace er {

struct Box {
  Box(const Box& other) = delete;
  Box& operator=(const Box& other) = delete;

  Box() = default;
  Box(Box&& other) noexcept = default;
  Box& operator=(Box&& other) noexcept = default;

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

}  // namespace er
