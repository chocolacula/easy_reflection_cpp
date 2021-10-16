#include "er/serialization/yaml.h"

#include <ostream>

#include "../writers/stream_writer.h"
#include "../writers/string_writer.h"
#include "er/reflection/reflection.h"
#include "er/types/all_types.h"
#include "parser_yaml.h"

using namespace rr::serialization;

inline void append(IWriter* writer, std::string_view str) {
  writer->write(str.data(), str.size());
}

inline void append(IWriter* writer, char ch) {
  writer->write(ch);
}

void serialize_recursive(IWriter* writer, const TypeInfo& info, int indent) {
  info.match(
      [writer, indent](const Object& o) {
        append(writer, '\n');
        for (auto&& record : o.get_fields()) {
          append(writer, std::string(indent, ' '));
          append(writer, record.first);
          append(writer, ": ");

          auto field_info = reflection::reflect(record.second.var());
          serialize_recursive(writer, field_info, indent + 2);
          append(writer, '\n');
        }
        writer->step_back(1);
      },
      [writer](const Bool& b) { append(writer, b.to_string()); },       //
      [writer](const Integer& i) { append(writer, i.to_string()); },    //
      [writer](const Floating& f) { append(writer, f.to_string(6)); },  //
      [writer](const String& s) { append(writer, s.get()); },           //
      [writer](const Enum& e) { append(writer, e.to_string()); },       //
      [writer, indent](const Map& m) {
        if (m.size() == 0) {
          append(writer, "{}\n");
          return;
        }

        append(writer, '\n');
        m.for_each([writer, indent](Var key, Var value) {
          append(writer, std::string(indent, ' '));

          auto key_info = reflection::reflect(key);

          if (key_info.is<Object>() || key_info.is<Map>() || key_info.is<Array>() || key_info.is<Sequence>()) {
            append(writer, "?");
            serialize_recursive(writer, key_info, indent + 2);
            append(writer, "\n");
            append(writer, std::string(indent, ' '));
            append(writer, ":");
          } else {
            // for a simple object indention doesn't matter
            serialize_recursive(writer, key_info, indent);
            append(writer, ": ");
          }

          auto value_info = reflection::reflect(value);
          serialize_recursive(writer, value_info, indent + 2);

          append(writer, '\n');
        });
        writer->step_back(1);
      },
      [writer, indent](const auto& as) {  // Array or Sequence
        if (as.size() == 0) {
          append(writer, "[]\n");
          return;
        }

        append(writer, '\n');
        as.for_each([writer, indent](Var entry) {
          append(writer, std::string(indent, ' '));
          append(writer, "- ");

          auto entry_info = reflection::reflect(entry);
          serialize_recursive(writer, entry_info, indent + 2);

          append(writer, '\n');
        });
        writer->step_back(1);
      });
}

void yaml::serialize(std::string* str, Var var) {
  StringWriter string_w(str);
  auto info = reflection::reflect(var);

  serialize_recursive(&string_w, info, 0);
}

void yaml::serialize(std::ostream& stream, Var var) {
  StreamWriter stream_w(stream);
  auto info = reflection::reflect(var);

  serialize_recursive(&stream_w, info, 0);
}

Expected<None> yaml::deserialize(Var var, std::string_view str) {
  ParserYaml parser(str.data(), str.size());
  auto info = reflection::reflect(var);

  return parser.deserialize(&info);
}

Expected<None> yaml::deserialize(Var var, std::istream& stream) {
  ParserYaml parser(stream);
  auto info = reflection::reflect(var);

  return parser.deserialize(&info);
}