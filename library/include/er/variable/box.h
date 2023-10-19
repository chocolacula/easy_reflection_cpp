#pragma once

#include <memory_resource>

#include "er/alloc/alloc.h"
#include "er/tools/sizeof.h"
#include "var.h"

namespace er {

struct Box {
  Box(const Box& other) = delete;
  Box& operator=(const Box& other) = delete;

  Box(palloc_t* alloc = &default_alloc) : _alloc(alloc){};
  Box(Box&& other) noexcept = default;
  Box& operator=(Box&& other) noexcept = default;

  explicit Box(TypeId id, palloc_t* alloc = &default_alloc);
  ~Box();

  Var var();
  Box clone();

  bool uses_heap() const;

 private:
  Var _var;
  palloc_t* _alloc;

  // max size of stack memory for dynamic allocation optimization
  // static const size_t kMemSize = Sizeof<std::unordered_map<int, int>, std::map<int, int>>::max();
  // std::array<uint8_t, kMemSize> _buff;
};

}  // namespace er
