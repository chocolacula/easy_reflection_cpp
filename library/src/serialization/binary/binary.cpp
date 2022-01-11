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

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, GroupWriter* writer);

inline void serialize_recursive(GroupWriter* writer, const TypeInfo& info) {

  auto k = info.get_kind();

  switch (k) {
    case TypeInfo::Kind::kObject:
      for (auto&& record : info.unsafe_get<Object>().get_fields()) {
        // skip name in record.first;
        auto field_info = reflection::reflect(record.second.var());
        serialize_recursive(writer, field_info);
      }
      break;
    case TypeInfo::Kind::kBool:
      writer->write(info.unsafe_get<Bool>().get());
      break;
    case TypeInfo::Kind::kInteger: {
      auto i = info.unsafe_get<Integer>();
      writer->write(i.var().raw(), i.size(), i.is_signed());
    } break;
    case TypeInfo::Kind::kFloating:
      writer->write(info.unsafe_get<Floating>().get());
      break;
    case TypeInfo::Kind::kString:
      writer->write(info.unsafe_get<String>().get());
      break;
    case TypeInfo::Kind::kEnum:
      writer->write(info.unsafe_get<Enum>().to_string());
      break;
    case TypeInfo::Kind::kMap: {
      auto m = info.unsafe_get<Map>();

      writer->write(m.size());

      auto key_info = reflection::reflect(Var(nullptr, m.key_type(), false));
      auto val_info = reflection::reflect(Var(nullptr, m.val_type(), false));
      m.unsafe_for_each([writer, &key_info, &val_info](void* key, void* val) {
        key_info.unsafe_assign(key);
        serialize_recursive(writer, key_info);

        val_info.unsafe_assign(val);
        serialize_recursive(writer, val_info);
      });
    } break;
    case TypeInfo::Kind::kArray:
      serialize_sequence(info.unsafe_get<Array>(), writer);
      break;
    case TypeInfo::Kind::kSequence:
      serialize_sequence(info.unsafe_get<Sequence>(), writer);
      break;
  }
}

template <typename SeqT>
inline void serialize_sequence(const SeqT& seq, GroupWriter* writer) {
  writer->write(seq.size());

  auto info = reflection::reflect(Var(nullptr, seq.nested_type(), false));
  seq.unsafe_for_each([writer, &info](void* ptr) {
    info.unsafe_assign(ptr);
    serialize_recursive(writer, info);
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
  auto k = info->get_kind();

  switch (k) {
    case TypeInfo::Kind::kObject:
      for (auto&& record : info->unsafe_get<Object>().get_fields()) {
        // skip name in record.first;
        auto field_info = reflection::reflect(record.second.var());
        deserialize_recursive(&field_info, reader);
      }
      break;
    case TypeInfo::Kind::kBool:
      info->unsafe_get<Bool>().set(reader.read_unsigned() == 1);
      break;
    case TypeInfo::Kind::kInteger: {
      auto i = info->unsafe_get<Integer>();
      if (i.is_signed()) {
        i.set_signed(reader.read_signeg());
      } else {
        i.set_unsigned(reader.read_unsigned());
      }
    } break;
    case TypeInfo::Kind::kFloating:
      info->unsafe_get<Floating>().set(reader.read_float());
      break;
    case TypeInfo::Kind::kString:
      info->unsafe_get<String>().set(reader.read_string());
      break;
    case TypeInfo::Kind::kEnum:
      info->unsafe_get<Enum>().parse(reader.read_string());
      break;
    case TypeInfo::Kind::kMap: {
      auto m = info->unsafe_get<Map>();
      Box key_box(m.key_type());
      Box val_box(m.val_type());
      m.clear();

      auto n = reader.read_unsigned();
      for (auto i = 0; i < n; i++) {
        auto key_info = reflection::reflect(key_box.var());
        deserialize_recursive(&key_info, reader);

        auto value_info = reflection::reflect(val_box.var());
        deserialize_recursive(&value_info, reader);

        m.insert(key_box.var(), val_box.var());
      }
    } break;
    case TypeInfo::Kind::kArray: {
      auto a = info->unsafe_get<Array>();
      auto n = reader.read_unsigned();

      size_t i = 0;
      a.for_each([&reader, &i, n](Var entry) {
        if (i >= n) {
          return;
        }
        i++;

        auto entry_info = reflection::reflect(entry);
        deserialize_recursive(&entry_info, reader);
      });
    } break;
    case TypeInfo::Kind::kSequence: {
      auto s = info->unsafe_get<Sequence>();
      Box entry_box(s.nested_type());
      s.clear();

      auto n = reader.read_unsigned();
      for (auto i = 0; i < n; i++) {
        auto entry_info = reflection::reflect(entry_box.var());
        deserialize_recursive(&entry_info, reader);

        s.push(entry_box.var());
      }
    } break;
  }
}

Expected<None> binary::deserialize(Var var, const std::vector<uint8_t>& vector) {
  VectorReader vector_r(vector);
  GroupReader group_r(&vector_r);

  auto info = reflection::reflect(var);

  deserialize_recursive(&info, group_r);

  return None();
}

Expected<None> binary::deserialize(Var var, std::istream& stream) {
  StreamReader stream_r(stream);
  GroupReader group_r(&stream_r);

  auto info = reflection::reflect(var);

  deserialize_recursive(&info, group_r);

  return None();
}
