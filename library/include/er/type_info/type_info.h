#pragma once

#include <string>
#include <string_view>

#include "er/expected.h"
#include "er/variant/variant.h"

// all variants
#include "variants/array/array.h"
#include "variants/bool.h"
#include "variants/enum/enum.h"
#include "variants/floating/floating.h"
#include "variants/integer/integer.h"
#include "variants/map/map.h"
#include "variants/object/object.h"
#include "variants/sequence/sequence.h"
#include "variants/string.h"

namespace er {

#define BASE Variant<Bool, Integer, Floating, String, Enum, Object, Array, Sequence, Map>

/// The sum type contains information about nature of stored value
///
/// Primitive for int, float, std::string and others
/// Object for structs and classes
/// Sequence for arrays and containers with one generic parameter
/// Dictionary for associative container such as std::map
class TypeInfo : public BASE {
 public:
  TypeInfo(Bool value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Integer value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Floating value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(String value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Enum value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Object value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Array value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Sequence value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Map value) : BASE(value) {  // NOLINT implicit constructor
  }

  Expected<None> assign(Var var) {
    return match([=](auto&& v) { return v.assign(var); });
  }

  void unsafe_assign(void* ptr) {
    match([=](auto&& m) { m.unsafe_assign(ptr); });
  }

  Var var() {
    return match([](Array& a) -> Var { return a.own_var(); },     //
                 [](Sequence& s) -> Var { return s.own_var(); },  //
                 [](Map& m) -> Var { return m.own_var(); },       //
                 [](auto&& v) { return v.var(); });
  }

  enum class Kind {
    kBool = 0,
    kInteger = 1,
    kFloating = 2,
    kString = 3,
    kEnum = 4,
    kObject = 5,
    kArray = 6,
    kSequence = 7,
    kMap = 8
  };

  [[nodiscard]] Kind get_kind() const {
    return static_cast<Kind>(variant_idx());
  }
};

#undef BASE

}  // namespace er