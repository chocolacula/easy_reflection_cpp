#pragma once

#include <memory_resource>

namespace er {

using palloc_t = std::pmr::polymorphic_allocator<uint8_t>;

static inline palloc_t default_alloc;

}  // namespace er
