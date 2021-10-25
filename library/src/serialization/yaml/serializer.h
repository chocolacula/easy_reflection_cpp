#pragma once

#include <string_view>

#include "../writers/iwriter.h"
#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"

namespace er {

inline void append(IWriter* writer, std::string_view str) {
  writer->write(str.data(), str.size());
}

inline void append(IWriter* writer, char ch) {
  writer->write(ch);
}

struct NewLine {};

template <typename T>
inline void end_line(IWriter* writer) {
  // do nothing for types others than NewLine
}

template <>
inline void end_line<NewLine>(IWriter* writer) {
  append(writer, '\n');
}

template <typename T>
void serialize_recursive(IWriter* writer, const TypeInfo& info, int indent) {
  info.match(
      [writer, indent](const Object& o) {
        end_line<T>(writer);
        for (auto&& record : o.get_fields()) {
          append(writer, std::string(indent, ' '));
          append(writer, record.first);
          append(writer, ": ");

          auto field_info = reflection::reflect(record.second.var());
          serialize_recursive<NewLine>(writer, field_info, indent + 2);
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

        end_line<T>(writer);
        m.for_each([writer, indent](Var key, Var value) {
          append(writer, std::string(indent, ' '));

          auto key_info = reflection::reflect(key);

          if (key_info.is<Object>() || key_info.is<Map>() || key_info.is<Array>() || key_info.is<Sequence>()) {
            append(writer, "?");
            serialize_recursive<NewLine>(writer, key_info, indent + 2);
            append(writer, "\n");
            append(writer, std::string(indent, ' '));
            append(writer, ":");
          } else {
            // for a simple object indention doesn't matter
            serialize_recursive<NewLine>(writer, key_info, indent);
            append(writer, ": ");
          }

          auto value_info = reflection::reflect(value);
          serialize_recursive<NewLine>(writer, value_info, indent + 2);

          append(writer, '\n');
        });
        writer->step_back(1);
      },
      [writer, indent](const auto& as) {  // Array or Sequence
        if (as.size() == 0) {
          append(writer, "[]\n");
          return;
        }

        end_line<T>(writer);
        as.for_each([writer, indent](Var entry) {
          append(writer, std::string(indent, ' '));
          append(writer, "- ");

          auto entry_info = reflection::reflect(entry);
          serialize_recursive<NewLine>(writer, entry_info, indent + 2);

          append(writer, '\n');
        });
        writer->step_back(1);
      });
}

}  // namespace er