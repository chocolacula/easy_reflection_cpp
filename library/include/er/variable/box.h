#pragma once

#include <memory_resource>

#include "er/buff_alloc.h"
#include "er/tools/sizeof.h"
#include "var.h"

namespace er {

struct Box {
  Box(const Box& other) = delete;
  Box& operator=(const Box& other) = delete;

  Box(std::pmr::polymorphic_allocator<uint8_t>* alloc = &DefaultAlloc){};
  Box(Box&& other) noexcept = default;
  Box& operator=(Box&& other) noexcept = default;

  explicit Box(TypeId id, std::pmr::polymorphic_allocator<uint8_t>* alloc = &DefaultAlloc);
  ~Box();

  Var var();
  Box clone();

  bool uses_heap() const;

 private:
  Var _var;
  std::pmr::polymorphic_allocator<uint8_t>* _alloc;

  // max size of stack memory for dynamic allocation optimization
  // static const size_t kMemSize = Sizeof<std::unordered_map<int, int>, std::map<int, int>>::max();
  // std::array<uint8_t, kMemSize> _buff;
};

}  // namespace er
