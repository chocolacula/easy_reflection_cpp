#include "er/serialization/json.h"

#include "parser_json.h"

using namespace rr::serialization;

void json::serialize(const TypeInfo& info, std::string* result) {
  info.match(
      [result](const Object& o) {
        *result += '{';
        for (auto&& record : o.get_fields()) {
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

Expected<None> json::deserialize(std::string_view str, TypeInfo* info) {
  ParserJson parser(str.data(), str.size());
  return parser.deserialize(info);
}

Expected<None> json::deserialize(std::istream& stream, TypeInfo* info) {
  ParserJson parser(stream);
  return parser.deserialize(info);
}