#pragma once

#include <cstdint>
#include <memory_resource>

#include "er/type_info/type_info.h"
#include "er/variable/var.h"

namespace er {

struct Actions {
  using palloc_t = std::pmr::polymorphic_allocator<uint8_t>;

  constexpr Actions(TypeInfo (*reflect)(void*, bool),                            //
                    std::string_view (*get_name)(),                              //
                    size_t (*size)(),                                            //
                    uint8_t* (*call_new)(palloc_t* alloc, size_t n),             //
                    void (*call_delete)(palloc_t* alloc, uint8_t* p, size_t n),  //
                    void (*copy)(void*, const void*),                            //
                    void (*move)(void*, void*))
      : reflect(reflect),          //
        type_name(get_name),       //
        type_size(size),           //
        call_new(call_new),        //
        call_delete(call_delete),  //
        copy(copy),                //
        move(move) {
  }

  constexpr Actions(const Actions& other) = default;
  constexpr Actions& operator=(const Actions& other) = default;

  // there is no reason to move the struct

  TypeInfo (*reflect)(void*, bool);
  std::string_view (*type_name)();
  size_t (*type_size)();
  uint8_t* (*call_new)(palloc_t* alloc, size_t n);
  void (*call_delete)(palloc_t* alloc, uint8_t* p, size_t n);
  void (*copy)(void*, const void*);
  void (*move)(void*, void*);
};

}  // namespace er
