#pragma once

#include <string>
#include <string_view>

#include "../field_info.h"
#include "er/expected.h"
#include "er/variant/variant.h"
#include "variants/array/array.h"
#include "variants/bool.h"
#include "variants/enum/enum.h"
#include "variants/floating/floating.h"
#include "variants/integer/integer.h"
#include "variants/map/map.h"
#include "variants/object.h"
#include "variants/sequence/sequence.h"
#include "variants/string.h"

namespace rr {

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

  std::string_view what_is() {
    return match([](Bool&) -> std::string_view { return "Bool"; },          //
                 [](Integer&) -> std::string_view { return "Integer"; },    //
                 [](Floating&) -> std::string_view { return "Float"; },     //
                 [](String&) -> std::string_view { return "String"; },      //
                 [](Enum&) -> std::string_view { return "Enum"; },          //
                 [](Object&) -> std::string_view { return "Object"; },      //
                 [](Array&) -> std::string_view { return "Array"; },        //
                 [](Sequence&) -> std::string_view { return "Sequence"; },  //
                 [](Map&) -> std::string_view { return "Map"; });
  }

  Var var() {
    return match([](Bool& b) -> Var { return b.var(); },          //
                 [](Integer& i) -> Var { return i.var(); },       //
                 [](Floating& f) -> Var { return f.var(); },      //
                 [](String& s) -> Var { return s.var(); },        //
                 [](Enum& e) -> Var { return e.var(); },          //
                 [](Object& o) -> Var { return o.var(); },        //
                 [](Array& a) -> Var { return a.own_var(); },     //
                 [](Sequence& s) -> Var { return s.own_var(); },  //
                 [](Map& m) -> Var { return m.own_var(); });
  }
};

#undef BASE

}  // namespace rr