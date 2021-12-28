#pragma once

#include <string_view>

#include "../writers/iwriter.h"
#include "er/reflection/reflection.h"
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

inline bool is_complex(TypeInfo::Kind kind) {
  return static_cast<int>(kind) >= static_cast<int>(TypeInfo::Kind::kObject);
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer, int indent);

void serialize_recursive(IWriter* writer, const TypeInfo& info, int indent) {
  auto k = info.get_kind();

  switch (k) {
    case TypeInfo::Kind::kBool:
      append(writer, info.unsafe_get<Bool>().to_string());
      break;
    case TypeInfo::Kind::kInteger:
      append(writer, info.unsafe_get<Integer>().to_string());
      break;
    case TypeInfo::Kind::kFloating:
      append(writer, info.unsafe_get<Floating>().to_string(6));
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
#pragma unroll 5
      for (auto&& record : o.get_fields()) {

        if (!is_first) {
          append_indention(writer, indent);
        }
        is_first = false;

        append(writer, record.first);

        auto field_info = reflection::reflect(record.second.var());

        if (is_complex(field_info.get_kind())) {
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

      void (*ser_val)(IWriter * writer, int indent, const TypeInfo& info);
      void (*ser_key)(IWriter * writer, int indent, const TypeInfo& info);

      if (is_complex(key_info.get_kind())) {
        ser_key = ser_complex_key;
        ser_val = ser_normal_val;
      } else {
        ser_key = ser_normal_key;
        if (is_complex(val_info.get_kind())) {
          ser_val = ser_complex_val;
        } else {
          ser_val = ser_normal_val;
        }
      }

      bool is_first = true;
      m.unsafe_for_each([writer,     //
                         indent,     //
                         &key_info,  //
                         &val_info,  //
                         ser_key,    //
                         ser_val,    //
                         &is_first](void* key, void* value) {
        if (!is_first) {
          append_indention(writer, indent);
        }
        is_first = false;

        key_info.unsafe_assign(key);
        ser_key(writer, indent, key_info);

        val_info.unsafe_assign(value);
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
  }
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, IWriter* writer, int indent) {
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