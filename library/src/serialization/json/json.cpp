#include "er/serialization/json.h"

#include <string_view>

#include "../writers/stream_writer.h"
#include "../writers/string_writer.h"
#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"
#include "parser_json.h"

using namespace er::serialization;

inline void append(IWriter* writer, std::string_view str) {
  writer->write(str.data(), str.size());
}

inline void append(IWriter* writer, char ch) {
  writer->write(ch);
}

void serialize_recursive(IWriter* writer, const TypeInfo& info) {
  info.match(
      [writer](const Object& o) {
        auto fields = o.get_fields();

        if (fields.size() == 0) {
          append(writer, "{}");
          return;
        }

        append(writer, '{');
        for (auto&& record : fields) {
          append(writer, '"');
          append(writer, record.first);
          append(writer, "\":");

          auto field_info = reflection::reflect(record.second.var());
          serialize_recursive(writer, field_info);
          append(writer, ',');
        }
        writer->step_back(1);
        append(writer, '}');
      },
      [writer](const Bool& b) { append(writer, b.to_string()); },     //
      [writer](const Integer& i) { append(writer, i.to_string()); },  //
      [writer](const Floating& f) { append(writer, f.to_string(6)); },
      [writer](const String& s) {
        append(writer, '"');
        append(writer, s.get());
        append(writer, '"');
      },
      [writer](const Enum& e) {
        append(writer, '"');
        append(writer, e.to_string());
        append(writer, '"');
      },
      [writer](const Map& m) {
        if (m.size() == 0) {
          append(writer, "[]");
          return;
        }

        append(writer, '[');
        m.for_each([writer](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          append(writer, "{\"key\":");
          serialize_recursive(writer, key_info);

          append(writer, ',');

          auto value_info = reflection::reflect(value);
          append(writer, "\"val\":");
          serialize_recursive(writer, value_info);

          append(writer, "},");
        });
        writer->step_back(1);
        append(writer, ']');
      },
      [writer](const auto& as) {  // Array or Sequence
        if (as.size() == 0) {
          append(writer, "[]");
          return;
        }

        append(writer, '[');
        as.for_each([writer](Var entry) {
          auto entry_info = reflection::reflect(entry);

          serialize_recursive(writer, entry_info);

          append(writer, ',');
        });
        writer->step_back(1);
        append(writer, ']');
      });
}

void json::serialize(std::string* str, Var var) {
  StringWriter string_w(str);
  auto info = reflection::reflect(var);

  serialize_recursive(&string_w, info);
}

void json::serialize(std::ostream& stream, Var var) {
  StreamWriter stream_w(stream);
  auto info = reflection::reflect(var);

  serialize_recursive(&stream_w, info);
}

Expected<None> json::deserialize(Var var, std::string_view str) {
  ParserJson parser(str.data(), str.size());
  auto info = reflection::reflect(var);

  return parser.deserialize(&info);
}

Expected<None> json::deserialize(Var var, std::istream& stream) {
  ParserJson parser(stream);
  auto info = reflection::reflect(var);

  return parser.deserialize(&info);
}