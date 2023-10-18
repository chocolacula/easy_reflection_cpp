#pragma once

#include "er/type_info/type_info.h"
#include "er/variable/var.h"

namespace er {

struct Actions {
  constexpr Actions(TypeInfo (*reflect)(void*, bool),  //
                    std::string_view (*get_name)(),    //
                    size_t (*size)(),                  //
                    void (*construct)(void* p),        //
                    void (*destroy)(void* p),          //
                    void (*copy)(void*, const void*),  //
                    void (*move)(void*, void*))
      : reflect(reflect),      //
        type_name(get_name),   //
        type_size(size),       //
        construct(construct),  //
        destroy(destroy),      //
        copy(copy),            //
        move(move) {
  }

  constexpr Actions(const Actions& other) = default;
  constexpr Actions& operator=(const Actions& other) = default;

  // there is no reason to move the struct

  TypeInfo (*reflect)(void*, bool);
  std::string_view (*type_name)();
  size_t (*type_size)();
  void (*construct)(void* p);
  void (*destroy)(void* p);
  void (*copy)(void*, const void*);
  void (*move)(void*, void*);
};

}  // namespace er
