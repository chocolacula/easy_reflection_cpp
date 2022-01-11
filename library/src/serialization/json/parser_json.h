#pragma once

#include <istream>

#include "er/expected.h"
#include "er/type_info/variants/array/array.h"
#include "er/type_info/variants/map/map.h"
#include "er/variable/var.h"
#include "lexer_json.yy.h"

namespace er {

struct TypeInfo;

class ParserJson : rf_json::LexerJson {
 public:
  ParserJson(const char* input, size_t input_size);
  explicit ParserJson(std::istream& stream);

  Expected<None> deserialize(TypeInfo* info);

 private:
  Expected<None> parse(TypeInfo* info, char token);
  Expected<None> parse_next(TypeInfo* info);

  Expected<None> parse_array(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add);

  static inline Expected<None> add_to_array(Array& a, size_t i, Var var);

  Expected<None> parse_object(TypeInfo* info);

  Expected<None> parse_map(Map& map);

  inline char next();

  inline Error error(const char* str);
  inline Error error_token(char token);
  inline Error error_match();

  inline Expected<None> parse_field(Var new_var);
  inline Expected<std::pair<std::string, std::string>> parse_tag(std::string_view str);
  static inline int64_t parse_int(std::string_view str);
  static inline double parse_double(std::string_view str);

  static const size_t kMaxLevel = 100;  // to keep things secure: max JSON nesting depth
  static const size_t kMaxArr = 1000;   // to keep things secure: max JSON array and object size
  size_t _level;
};

}  // namespace er
