#include "er/serialization/simd_json.h"

#include <string_view>

#include "../define_retry.h"
#include "er/reflection/reflection.h"
#include "er/tools/format.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/floating/floating.h"
#include "er/types/all_types.h"
#include "er/variable/box.h"
#include "simdjson.h"

using namespace er::serialization;
using namespace simdjson;

Expected<None> deserialize_recursive(TypeInfo* info, dom::element elem) {
  auto t = elem.type();

  switch (t) {
    case dom::element_type::BOOL:
      if (info->is<Bool>()) {
        info->unsafe_get<Bool>().set(elem.get_bool());
      } else {
        return Error(format("Cannot deserialize number to {}", info->get_kind_str()));
      }
      break;
    case dom::element_type::INT64:
      if (info->is<Integer>()) {
        auto i = info->unsafe_get<Integer>();
        if (i.is_signed()) {
          i.set_signed(elem.get_int64());
        } else {
          i.set_unsigned(elem.get_uint64());
        }
      } else {
        return Error(format("Cannot deserialize number to {}", info->get_kind_str()));
      }
      break;
    case dom::element_type::DOUBLE:
      if (info->is<Floating>()) {
        info->unsafe_get<Floating>().set(elem.get_double());
      } else {
        return Error(format("Cannot deserialize double to {}", info->get_kind_str()));
      }
      break;
    case dom::element_type::STRING:
      if (info->is<String>()) {
        info->unsafe_get<String>().set(elem.get_string());
      } else if (info->is<Enum>()) {
        info->unsafe_get<Enum>().parse(elem.get_string());
      } else if (info->is<Floating>()) {
        double v;
        auto str = elem.get_string().value_unsafe();
        if (str == "-inf") {
          v = -std::numeric_limits<double>::infinity();
        } else if (str == "inf") {
          v = std::numeric_limits<double>::infinity();
        } else if (str == "nan") {
          v = std::nan("");
        } else {
          return Error(format("Expected -inf, inf, nan but {} reached", str));
        }
        info->unsafe_get<Floating>().set(v);
      } else {
        return Error(format("Cannot deserialize string to {}", info->get_kind_str()));
      }
      break;
    case dom::element_type::ARRAY:
      if (info->is<Array>()) {
        auto a = info->unsafe_get<Array>();

        Box box(a.nested_type());
        auto entry_info = reflection::reflect(box.var());

        auto i = 0;
        for (auto&& e : elem.get_array()) {
          __retry(deserialize_recursive(&entry_info, e));

          auto item = a.at(i).unwrap();
          reflection::copy(item, box.var());
          i++;
        }
      } else if (info->is<Sequence>()) {
        auto s = info->unsafe_get<Sequence>();
        s.clear();

        Box box(s.nested_type());
        auto entry_info = reflection::reflect(box.var());

        for (auto&& e : elem.get_array()) {
          __retry(deserialize_recursive(&entry_info, e));

          s.push(box.var());
        }
      } else if (info->is<Map>()) {
        auto m = info->unsafe_get<Map>();
        m.clear();

        Box key_box(m.key_type());
        Box val_box(m.val_type());

        auto key_info = reflection::reflect(key_box.var());
        auto val_info = reflection::reflect(val_box.var());
        for (auto&& entry : elem.get_array()) {
          auto key = entry["key"].value_unsafe();
          auto val = entry["val"].value_unsafe();

          __retry(deserialize_recursive(&key_info, key));
          __retry(deserialize_recursive(&val_info, val));

          m.insert(key_box.var(), val_box.var());
        }
      } else {
        return Error(format("Cannot deserialize to {}", info->get_kind_str()));
      }
      break;
    case dom::element_type::OBJECT:
      if (info->is<Object>()) {
        auto o = info->unsafe_get<Object>();

        for (auto [k, v] : elem.get_object()) {
          auto ex = o.get_field(k);
          __retry(ex);

          auto field = ex.unwrap();
          auto field_info = reflection::reflect(field.var());

          __retry(deserialize_recursive(&field_info, v));
        }
      } else {
        return Error(format("Expected Object but {} found", info->get_kind_str()));
      }
      break;
    default:
      // found null, do nothing
      break;
  }
  return None();
}

Expected<None> simd_json::deserialize(Var var, std::string_view str) {
  dom::parser parser;
  dom::element elem = parser.parse(padded_string(str));

  auto info = reflection::reflect(var);
  return deserialize_recursive(&info, elem);
}
