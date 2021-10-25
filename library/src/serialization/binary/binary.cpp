#include "er/serialization/binary.h"

#include <iostream>

#include "../readers/stream_reader.h"
#include "../readers/vector_reader.h"
#include "../writers/stream_writer.h"
#include "../writers/vector_writer.h"
#include "er/reflection/reflection.h"
#include "er/types/all_types.h"
#include "er/variable/box.h"
#include "group_reader.h"
#include "group_writer.h"

using namespace er::serialization;

inline void serialize_recursive(GroupWriter* writer, const TypeInfo& info) {
  info.match(
      [writer](const Object& o) {
        for (auto&& record : o.get_fields()) {
          // skip name in record.first;
          auto field_info = reflection::reflect(record.second.var());
          serialize_recursive(writer, field_info);
        }
      },
      [writer](const Bool& b) { writer->write(b.get()); },                                    //
      [writer](const Integer& i) { writer->write(i.var().raw(), i.size(), i.is_signed()); },  //
      [writer](const Floating& f) { writer->write(f.get()); },                                //
      [writer](const String& s) {
        auto str = s.get();

        writer->write(str.size());
        for (auto&& ch : str) {
          writer->write(ch);
        }
      },
      [writer](const Enum& e) {
        auto str = e.to_string();

        writer->write(str.size());
        for (auto&& ch : str) {
          writer->write(ch);
        }
      },
      [writer](const Map& m) {
        writer->write(m.size());

        m.for_each([writer](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          serialize_recursive(writer, key_info);

          auto value_info = reflection::reflect(value);
          serialize_recursive(writer, value_info);
        });
      },
      [writer](const auto& as) {  // Array or Sequence
        writer->write(as.size());

        as.for_each([writer](Var entry) {
          auto entry_info = reflection::reflect(entry);
          serialize_recursive(writer, entry_info);
        });
      });
}

void binary::serialize(std::vector<uint8_t>* vector, Var var) {
  VectorWriter vector_w(vector);
  GroupWriter group_w(&vector_w);

  auto info = reflection::reflect(var);

  serialize_recursive(&group_w, info);
}

void binary::serialize(std::ostream& stream, Var var) {
  StreamWriter stream_w(stream);
  GroupWriter group_w(&stream_w);

  auto info = reflection::reflect(var);

  serialize_recursive(&group_w, info);
}

inline void deserialize_recursive(TypeInfo* info, const GroupReader& reader) {
  info->match(
      [&reader](Object& o) {
        for (auto&& record : o.get_fields()) {
          // skip name in record.first;
          auto field_info = reflection::reflect(record.second.var());
          deserialize_recursive(&field_info, reader);
        }
      },
      [&reader](Bool& b) { b.set(reader.read_integer() == 1); },  //
      [&reader](Integer& i) { i.set(reader.read_integer()); },    //
      [&reader](Floating& f) { f.set(reader.read_float()); },     //
      [&reader](String& s) {
        std::string str;
        auto n = reader.read_integer();

        for (auto i = 0; i < n; i++) {
          str.push_back(static_cast<char>(reader.read_integer()));
        }

        s.set(str);
      },
      [&reader](Enum& e) {
        std::string str;
        auto n = reader.read_integer();

        for (auto i = 0; i < n; i++) {
          str.push_back(static_cast<char>(reader.read_integer()));
        }

        e.parse(str);
      },
      [&reader](Map& m) {
        Box key_box(m.key_type());
        Box val_box(m.val_type());
        m.clear();

        auto n = reader.read_integer();
        for (auto i = 0; i < n; i++) {
          auto key_info = reflection::reflect(key_box.var());
          deserialize_recursive(&key_info, reader);

          auto value_info = reflection::reflect(val_box.var());
          deserialize_recursive(&value_info, reader);

          m.insert(key_box.var(), val_box.var());
        }
      },
      [&reader](Array& a) {
        auto n = reader.read_integer();

        size_t i = 0;
        a.for_each([&reader, &i, n](Var entry) {
          if (i >= n) {
            return;
          }
          i++;

          auto entry_info = reflection::reflect(entry);
          deserialize_recursive(&entry_info, reader);
        });
      },
      [&reader](Sequence& s) {
        Box entry_box(s.nested_type());
        s.clear();

        auto n = reader.read_integer();
        for (auto i = 0; i < n; i++) {
          auto entry_info = reflection::reflect(entry_box.var());
          deserialize_recursive(&entry_info, reader);

          s.push(entry_box.var());
        }
      });
}

Expected<None> binary::deserialize(Var var, const std::vector<uint8_t>& vector) {
  VectorReader vector_r(vector);
  GroupReader group_r(&vector_r);

  auto info = reflection::reflect(var);

  deserialize_recursive(&info, group_r);

  return None();
}

Expected<None> binary::deserialize(Var var, std::istream& stream) {
  StreamReader<8> stream_r(stream);
  GroupReader group_r(&stream_r);

  auto info = reflection::reflect(var);

  deserialize_recursive(&info, group_r);

  return None();
}
