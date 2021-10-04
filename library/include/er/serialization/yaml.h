#pragma once

#include <istream>
#include <string>

#include "er/expected.h"
#include "er/reflection/reflection.h"
#include "parsing/parser_yaml.h"

namespace rr::serialization::yaml {

template <typename T>
static Expected<T> from_string(std::string_view str) {
  ParserYaml parser(str.data(), str.size());

  T obj;
  auto info = reflection::reflect(&obj);
  auto exp = parser.deserialize(info);
  if (exp.is_error()) {
    return exp.error();
  }
  return obj;
}

template <typename T>
static Expected<T> from_stream(std::istream& stream) {
  ParserYaml parser(stream);

  T obj;
  auto info = reflection::reflect(&obj);
  auto exp = parser.deserialize(&info);
  if (exp.is_error()) {
    return exp.error();
  }
  return obj;
}

static void serialize(const TypeInfo& info, std::string* result, int indent);

template <typename T>
static Expected<std::string> to_string(const T& obj) {
  auto info = reflection::reflect(obj);

  std::string result;
  serialize(info, &result, 0);
  return result;
}

static void serialize(const TypeInfo& info, std::string* result, int indent) {
  info.match(
      [result, indent](const Object& o) {
        for (auto&& record : o.get_all_fields()) {
          if (record.second.access() != Access::kPublic) {
            continue;
          }

          *result += std::string(indent, ' ');
          *result += record.first;
          *result += ": ";

          auto field_info = reflection::reflect(record.second.var());
          serialize(field_info, result, indent + 2);
          *result += '\n';
        }
      },
      [result](const Bool& b) { *result += b.to_string(); },       //
      [result](const Integer& i) { *result += i.to_string(); },    //
      [result](const Floating& f) { *result += f.to_string(6); },  //
      [result](const String& s) { *result += s.get(); },           //
      [result](const Enum& e) { *result += e.to_string(); },       //
      [result, indent](const Map& m) {
        if (m.size() == 0) {
          *result += "{}\n";
          return;
        }
        m.for_each([result, indent](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          serialize(key_info, result, indent + 2);

          *result += ": ";

          auto value_info = reflection::reflect(value);
          serialize(value_info, result, indent + 2);

          *result += '\n';
        });
      },
      [result, indent](const auto& as) {  // Array or Sequence
        if (as.size() == 0) {
          *result += "[]\n";
          return;
        }
        *result += '\n';
        as.for_each([result, indent](Var entry) {
          *result += "  - ";
          auto entry_info = reflection::reflect(entry);
          serialize(entry_info, result, indent + 4);

          *result += '\n';
        });

        if ((*result)[result->size() - 1] == '\n') {
          result->pop_back();
        }
      });
}

}  // namespace rr::serialization::yaml