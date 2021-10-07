#include "er/serialization/yaml.h"

#include "parser_yaml.h"

using namespace rr::serialization;

void yaml::serialize(const TypeInfo& info, std::string* result, int indent) {
  info.match(
      [result, indent](const Object& o) {
        for (auto&& record : o.get_fields()) {
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

Expected<None> yaml::deserialize(std::string_view str, TypeInfo* info) {
  ParserYaml parser(str.data(), str.size());
  return parser.deserialize(info);
}

Expected<None> yaml::deserialize(std::istream& stream, TypeInfo* info) {
  ParserYaml parser(stream);
  return parser.deserialize(info);
}