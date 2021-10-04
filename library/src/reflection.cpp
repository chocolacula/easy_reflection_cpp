#include "er/reflection/reflection.h"

#include <cstddef>
#include <iostream>

#include "er/reflection/the_great_table.h"
#include "er/types/all_types.h"

using namespace rr;

TypeInfo reflection::reflect(Var variable) {
  return TheGreatTable::data()[variable.type().number()].reflect(const_cast<void*>(variable.raw()),
                                                                 variable.is_const());
}

void parse(const TypeInfo& info, std::string* result);

std::string reflection::sprint(const TypeInfo& info) {
  std::string result;
  parse(info, &result);
  return result;
}

std::string reflection::sprint(Var var) {
  return sprint(reflect(var));
}

void reflection::print(const TypeInfo& info) {
  std::cout << sprint(info) << std::flush;
}

void reflection::print(Var var) {
  print(reflect(var));
}

std::string_view reflection::type_name(TypeId id) {
  return TheGreatTable::data()[id.number()].type_name();
}

size_t reflection::type_size(TypeId id) {
  return TheGreatTable::data()[id.number()].type_size();
}

void* reflection::call_new(TypeId id, void* place, size_t place_size) {
  return TheGreatTable::data()[id.number()].call_new(place, place_size);
}

void reflection::call_delete(Var variable, bool in_place) {
  TheGreatTable::data()[variable.type().number()].call_delete(variable.raw_mut(), in_place);
}

Expected<None> reflection::copy(Var to, Var from) {
  if (to.is_const()) {
    return Error("Cannot assign to const value");
  }
  if (to.type() != from.type()) {
    return Error(format("Cannot copy {} to {}", type_name(from.type()), type_name(to.type())));
  }
  TheGreatTable::data()[to.type().number()].copy(to.raw_mut(), from.raw());
  return None();
}

void parse(const TypeInfo& info, std::string* result) {
  info.match(
      [result](const Object& o) {
        for (auto&& record : o.get_all_fields()) {
          // add the field name and trailing whitespace
          *result += record.first;
          *result += ": ";

          auto field_info = reflection::reflect(record.second.var());
          parse(field_info, result);
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
          parse(key_info, result);

          *result += ": ";

          auto value_info = reflection::reflect(value);
          parse(value_info, result);

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

          parse(entry_info, result);
          *result += ", ";
        });
        if ((*result)[result->size() - 2] == ',') {
          result->replace(result->size() - 2, 2, "]");
        } else {
          *result += "]";
        }
      });
}
