#pragma once

#include "er/reflection/reflection.h"
#include "er/type_info/type_info.h"
#include "er/type_info/variants/object/access.h"

namespace rr {

void sprint(const TypeInfo& info, std::string* result) {
  info.match(
      [result](const Object& o) {
        for (auto&& record : o.get_fields(Access::kAll)) {
          // add the field name and trailing whitespace
          *result += record.first;
          *result += ": ";

          auto field_info = reflection::reflect(record.second.var());
          sprint(field_info, result);
          *result += '\n';
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
      [result](const Map& m) {
        *result += "[";

        m.for_each([result](Var key, Var value) {
          auto key_info = reflection::reflect(key);
          sprint(key_info, result);

          *result += ": ";

          auto value_info = reflection::reflect(value);
          sprint(value_info, result);

          *result += ", ";
        });
        if ((*result)[result->size() - 2] == ',') {
          result->replace(result->size() - 2, 2, "]");
        } else {
          *result += "]";
        }
      },
      [result](const auto& as) {  // Array or Sequence
        *result += "[";

        as.for_each([result](Var entry) {
          auto entry_info = reflection::reflect(entry);

          sprint(entry_info, result);
          *result += ", ";
        });
        if ((*result)[result->size() - 2] == ',') {
          result->replace(result->size() - 2, 2, "]");
        } else {
          *result += "]";
        }
      });
}

}  // namespace rr
