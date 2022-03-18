#pragma once

#include "er/type_info/type_info.h"
#include "er/variable/var.h"

namespace er {

struct Actions {

  constexpr Actions(TypeInfo (*reflect)(void*, bool),  //
                    std::string_view (*get_name)(),    //
                    size_t (*size)(),                  //
                    void* (*call_new)(void*, size_t),  //
                    void (*call_delete)(void*, bool),  //
                    void (*copy)(void*, const void*))
      : reflect(reflect),     //
        type_name(get_name),  //
        type_size(size),      //
        call_new(call_new),   //
        call_delete(call_delete),
        copy(copy) {
  }

  constexpr Actions(const Actions& other) = default;
  constexpr Actions& operator=(const Actions& other) = default;

  // there is no reason to move the struct

  TypeInfo (*reflect)(void*, bool);
  std::string_view (*type_name)();
  size_t (*type_size)();
  void* (*call_new)(void*, size_t);
  void (*call_delete)(void*, bool);
  void (*copy)(void*, const void*);
};

}  // namespace er
