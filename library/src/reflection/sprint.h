#pragma once

#include <string>

#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/object/field_attributes.h"

namespace er {

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
      [result](const Bool& b) { *result += b.to_string(); },     //
      [result](const Integer& i) { *result += i.to_string(); },  //
      [result](const Floating& f) { *result += f.to_string(); },
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
      [result, indention](const auto& as) {  // Array or Sequence
        if (as.size() == 0) {
          *result += "[]\n";
        }

        *result += "[";
        as.for_each([result, indention](Var entry) {
          auto entry_info = reflection::reflect(entry);

          sprint(entry_info, result, indention);
          *result += ", ";
        });
        result->resize(result->size() - 2);
        *result += "]";
      });
}

}  // namespace er
