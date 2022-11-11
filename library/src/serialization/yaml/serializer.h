#pragma once

#include <cmath>
#include <string_view>

#include "../writers/iwriter.h"
#include "er/error/error.h"
#include "er/reflection/reflection.h"
#include "er/tools/stringify.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/integer/integer.h"

namespace er {

inline void append(IWriter* writer, std::string_view str) {
  writer->write(str.data(), str.size());
}

inline void append(IWriter* writer, char ch) {
  writer->write(ch);
}

inline void append_indention(IWriter* writer, int num) {
  for (auto i = 0; i < num; i++) {
    writer->write(' ');
  }
}

inline bool is_complex(const TypeInfo& info) {
  switch (info.get_kind()) {
    case TypeInfo::Kind::kObject:
      [[fallthrough]];
    case TypeInfo::Kind::kArray:
      [[fallthrough]];
    case TypeInfo::Kind::kSequence:
      [[fallthrough]];
    case TypeInfo::Kind::kMap:
      return true;
    case TypeInfo::Kind::kPointer: {
      auto p = info.unsafe_get<Pointer>();
      return p.get_nested().match_move(  //
          [](const Error& /*err*/) { return false; },
          [](Var var) {
            auto nested_info = reflection::reflect(var);
            return is_complex(nested_info);
          });
    }
    default:
      return false;
  }
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer, int indent);

std::string double_to_string(double value) {
  if (value == -std::numeric_limits<double>::infinity()) {
    return "!!float .-inf";
  }
  if (value == std::numeric_limits<double>::infinity()) {
    return "!!float .inf";
  }
  if (std::isnan(value)) {
    return "!!float .nan";
  }
  return to_string(value, 9);
}

void serialize_recursive(IWriter* writer, const TypeInfo& info, int indent) {
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
      append(writer, info.unsafe_get<String>().get());
      break;
    case TypeInfo::Kind::kEnum:
      append(writer, info.unsafe_get<Enum>().to_string());
      break;
    case TypeInfo::Kind::kObject: {
      const auto& o = info.unsafe_get<Object>();

      bool is_first = true;
      for (auto&& record : o.get_fields()) {

        if (!is_first) {
          append_indention(writer, indent);
        }
        is_first = false;

        append(writer, record.first);

        auto field_info = reflection::reflect(record.second.var());

        if (is_complex(field_info)) {
          append(writer, ":\n");
          append(writer, std::string(indent + 2, ' '));
          serialize_recursive(writer, field_info, indent + 2);

        } else {
          append(writer, ": ");
          serialize_recursive(writer, field_info, indent + 2);
        }

        append(writer, '\n');
      }
      writer->step_back(1);
      break;
    }
    case TypeInfo::Kind::kMap: {
      // TODO clear this hell
      const auto& m = info.unsafe_get<Map>();

      if (m.size() == 0) {
        append(writer, "{}\n");
        return;
      }

      const auto ser_complex_key = [](IWriter* writer, int indent, const TypeInfo& info) {
        append(writer, "? ");
        serialize_recursive(writer, info, indent + 2);
        append(writer, "\n");
        append_indention(writer, indent);
      };

      const auto ser_normal_key = [](IWriter* writer, int indent, const TypeInfo& info) {
        serialize_recursive(writer, info, indent);
      };

      const auto ser_complex_val = [](IWriter* writer, int indent, const TypeInfo& info) {
        append(writer, ":\n");
        append(writer, std::string(indent + 2, ' '));
        serialize_recursive(writer, info, indent + 2);
      };

      const auto ser_normal_val = [](IWriter* writer, int indent, const TypeInfo& info) {
        append(writer, ": ");
        serialize_recursive(writer, info, indent + 2);
      };

      auto key_info = reflection::reflect(Var(nullptr, m.key_type(), false));
      auto val_info = reflection::reflect(Var(nullptr, m.val_type(), false));

      bool is_first = true;
      m.unsafe_for_each([writer,                                                           //
                         indent,                                                           //
                         &key_info, &val_info,                                             //
                         &is_first,                                                        //
                         ser_complex_key, ser_normal_key, ser_complex_val, ser_normal_val  //
      ](void* key, void* value) {
        if (!is_first) {
          append_indention(writer, indent);
        }
        is_first = false;

        key_info.unsafe_assign(key);
        val_info.unsafe_assign(value);

        void (*ser_val)(IWriter * writer, int indent, const TypeInfo& info);
        void (*ser_key)(IWriter * writer, int indent, const TypeInfo& info);

        if (is_complex(key_info)) {
          ser_key = ser_complex_key;
          ser_val = ser_normal_val;
        } else {
          ser_key = ser_normal_key;
          if (is_complex(val_info)) {
            ser_val = ser_complex_val;
          } else {
            ser_val = ser_normal_val;
          }
        }

        ser_key(writer, indent, key_info);
        ser_val(writer, indent, val_info);

        append(writer, '\n');
      });
      writer->step_back(1);
      break;
    }
    case TypeInfo::Kind::kArray:
      serialize_sequence(info.unsafe_get<Array>(), writer, indent);
      break;
    case TypeInfo::Kind::kSequence:
      serialize_sequence(info.unsafe_get<Sequence>(), writer, indent);
      break;
    case TypeInfo::Kind::kPointer: {
      auto p = info.unsafe_get<Pointer>();
      p.get_nested().match_move(  //
          [writer](const Error& /*err*/) { append(writer, "null"); },
          [writer, indent](Var var) {
            auto info = reflection::reflect(var);
            serialize_recursive(writer, info, indent);
          });
    } break;
  }
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer, int indent) {
  // TODO add possibility to write short sequences in one line
  if (seq.size() == 0) {
    append(writer, "[]\n");
    return;
  }

  auto info = reflection::reflect(Var(nullptr, seq.nested_type(), false));

  bool is_first = true;
  seq.unsafe_for_each([writer, indent, &info, &is_first](void* ptr) {
    if (!is_first) {
      append_indention(writer, indent);
    }
    is_first = false;

    append(writer, "- ");

    info.unsafe_assign(ptr);
    serialize_recursive(writer, info, indent + 2);

    append(writer, '\n');
  });
  writer->step_back(1);
}

}  // namespace er
