#include "er/serialization/json.h"

#include <cmath>
#include <string_view>

#include "../writers/iwriter.h"
#include "../writers/stream_writer.h"
#include "../writers/string_writer.h"
#include "er/reflection/reflection.h"
#include "er/tools/stringify.h"
#include "er/type_info/type_info.h"
#include "er/types/all_types.h"
#include "parser_json.h"

using namespace er::serialization;

inline void append(IWriter* writer, std::string_view str) {
  writer->write(str.data(), str.size());
}

inline void append(IWriter* writer, char ch) {
  writer->write(ch);
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer);

std::string double_to_string(double value) {
  if (value == -std::numeric_limits<double>::infinity()) {
    return "\"-inf\"";
  }
  if (value == std::numeric_limits<double>::infinity()) {
    return "\"inf\"";
  }
  if (std::isnan(value)) {
    return "\"nan\"";
  }
  return to_string(value, 9);
}

void serialize_recursive(IWriter* writer, const TypeInfo& info) {
  auto k = info.get_kind();

  switch (k) {
    case TypeInfo::Kind::kBool:
      append(writer, to_string(info.unsafe_get<Bool>().get()));
      break;
    case TypeInfo::Kind::kInteger: {
      auto i = info.unsafe_get<Integer>();
      if (i.is_signed()) {
        append(writer, to_string(i.as_signed()));
      } else {
        append(writer, to_string(i.as_unsigned()));
      }
    } break;
    case TypeInfo::Kind::kFloating:
      append(writer, double_to_string(info.unsafe_get<Floating>().get()));
      break;
    case TypeInfo::Kind::kString:
      append(writer, '"');
      append(writer, info.unsafe_get<String>().get());
      append(writer, '"');
      break;
    case TypeInfo::Kind::kEnum:
      append(writer, '"');
      append(writer, info.unsafe_get<Enum>().to_string());
      append(writer, '"');
      break;
    case TypeInfo::Kind::kObject: {
      const auto& o = info.unsafe_get<Object>();

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
      break;
    }
    case TypeInfo::Kind::kArray:
      serialize_sequence(info.unsafe_get<Array>(), writer);
      break;
    case TypeInfo::Kind::kSequence:
      serialize_sequence(info.unsafe_get<Sequence>(), writer);
      break;
    case TypeInfo::Kind::kMap: {
      const auto& m = info.unsafe_get<Map>();

      if (m.size() == 0) {
        append(writer, "[]");
        return;
      }
      append(writer, '[');

      auto key_info = reflection::reflect(Var(nullptr, m.key_type(), false));
      auto val_info = reflection::reflect(Var(nullptr, m.val_type(), false));
      m.unsafe_for_each([writer, &key_info, &val_info](void* key, void* val) {
        append(writer, "{\"key\":");
        key_info.unsafe_assign(key);
        serialize_recursive(writer, key_info);

        append(writer, ',');

        append(writer, "\"val\":");
        val_info.unsafe_assign(val);
        serialize_recursive(writer, val_info);

        append(writer, "},");
      });
      writer->step_back(1);
      append(writer, ']');
      break;
    }
  }
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer) {
  if (seq.size() == 0) {
    append(writer, "[]");
    return;
  }
  append(writer, '[');

  auto info = reflection::reflect(Var(nullptr, seq.nested_type(), false));
  seq.unsafe_for_each([writer, &info](void* ptr) {
    info.unsafe_assign(ptr);
    serialize_recursive(writer, info);

    append(writer, ',');
  });
  writer->step_back(1);
  append(writer, ']');
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