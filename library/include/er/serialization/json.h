#pragma once

#include <istream>

#include "er/expected.h"
#include "er/reflection/reflection.h"
#include "parsing/parser_json.h"

namespace rr::serialization::json {

template <typename T>
static Expected<T> from_string(std::string_view str) {
  ParserJson parser(str.data(), str.size());

  T obj;
  auto info = reflection::reflect(&obj);
  auto exp = parser.deserialize(&info);
  if (exp.is_error()) {
    return exp.error();
  }
  return obj;
}

template <typename T>
static Expected<T> from_stream(std::istream& stream) {
  ParserJson parser(stream);

  T obj;
  auto info = reflection::reflect(&obj);
  auto exp = parser.deserialize(&info);
  if (exp.is_error()) {
    return exp.error();
  }
  return obj;
}

static void serialize(const TypeInfo& info, std::string* result);

template <typename T>
static Expected<std::string> to_string(const T& obj) {
  auto info = reflection::reflect(obj);

  std::string result;
  serialize(info, &result);
  return result;
}

static void serialize(const TypeInfo& info, std::string* result) {
  info.match(
      [result](const Object& o) {
        *result += '{';
        for (auto&& record : o.get_all_fields()) {
          if (record.second.access() != Access::kPublic) {
            continue;
          }

          *result += '"';
          *result += record.first;
          *result += "\":";

          auto field_info = reflection::reflect(record.second.var());
          serialize(field_info, result);
          *result += ',';
        }

        if ((*result)[result->size() - 1] == ',') {
          (*result)[result->size() - 1] = '}';
        } else {
          *result += '}';
        }
      },
      [result](const Bool& b) { *result += b.to_string(); },     //
      [result](const Integer& i) { *result += i.to_string(); },  //
      [result](const Floating& f) { *result += f.to_string(6); },
      [result](const String& s) {
        *result += '"';
        *result += s.get();
        *result += '"';
      },
      [result](const Enum& e) {
        *result += '"';
        *result += e.to_string();
        *result += '"';
      },
      [result](const Map& m) {
        *result += '[';

        m.for_each([result](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          *result += "{\"key\":";
          serialize(key_info, result);

          *result += ',';

          auto value_info = reflection::reflect(value);
          *result += "\"val\":";
          serialize(value_info, result);

          *result += "},";
        });

        if ((*result)[result->size() - 1] == ',') {
          (*result)[result->size() - 1] = ']';
        } else {
          *result += ']';
        }
      },
      [result](const auto& as) {  // Array or Sequence
        *result += "[";

        as.for_each([result](Var entry) {
          auto entry_info = reflection::reflect(entry);

          serialize(entry_info, result);
          *result += ",";
        });

        if ((*result)[result->size() - 1] == ',') {
          (*result)[result->size() - 1] = ']';
        } else {
          *result += ']';
        }
      });
}

}  // namespace rr::serialization::json