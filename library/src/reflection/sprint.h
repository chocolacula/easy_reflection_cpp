#pragma once

#include <string>

#include "er/reflection/reflection.h"
#include "er/tools/stringify.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/object/field_attributes.h"

namespace er {

template <typename T>
void print_sequence(const T& sequence, std::string* result, int indention);

void sprint(const TypeInfo& info, std::string* result, int indention) {
  info.match(
      [result, indention](const Object& o) {
        if (!result->empty()) {
          *result += '\n';
        }
        for (auto&& record : o.get_fields(FieldAttributes::kAll)) {
          // indent a row
          *result += std::string(indention, ' ');
          // add the field name and trailing whitespace
          *result += record.first;
          *result += ": ";

          auto field_info = reflection::reflect(record.second.var());
          sprint(field_info, result, indention + 2);

          if (result->back() != '\n') {
            *result += '\n';
          }
        }
      },
      [result](const Bool& b) { *result += to_string(b.get()); },  //
      [result](const Integer& i) {
        if (i.is_signed()) {
          *result += to_string(i.as_signed());
        } else {
          *result += to_string(i.as_unsigned());
        }
      },
      [result](const Floating& f) { *result += to_string(f.get(), 2); },
      [result](const String& s) {
        *result += "'";
        *result += s.get();
        *result += "'";
      },
      [result](const Enum& e) { *result += e.to_string(); },
      [result, indention](const Map& m) {
        if (m.size() == 0) {
          *result += "[]\n";
          return;
        }

        *result += "[";
        m.for_each([result, indention](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          sprint(key_info, result, indention);

          if (result->back() == '\n') {
            *result += std::string(indention, ' ');
          }

          *result += ": ";

          auto value_info = reflection::reflect(value);
          sprint(value_info, result, indention);

          *result += ", ";
        });
        result->resize(result->size() - 2);
        *result += "]";
      },
      [result, indention](const Array& a) { print_sequence(a, result, indention); },
      [result, indention](const Sequence& s) { print_sequence(s, result, indention); },
      [result, indention](const Pointer& p) {
        auto nested_ptr = p.get_nested();
        nested_ptr.match_move(  //
            [result](const Error& /*err*/) { *result += "nullptr"; },
            [result, indention](Var var) {
              auto info = reflection::reflect(var);
              sprint(info, result, indention);
            });
      });
}

template <typename T>
void print_sequence(const T& sequence, std::string* result, int indention) {
  if (sequence.size() == 0) {
    *result += "[]\n";
    return;
  }

  *result += "[";
  sequence.for_each([result, indention](Var entry) {
    auto entry_info = reflection::reflect(entry);

    sprint(entry_info, result, indention);
    *result += ", ";
  });
  result->resize(result->size() - 2);
  *result += "]";
}

}  // namespace er
