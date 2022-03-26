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
  Expected<None> parse_object(TypeInfo* info);
  Expected<None> parse_map(Map& map);

  inline Error error(const char* str);
  inline Error error_token(char token);
  inline Error error_match();

  inline Expected<std::pair<std::string, std::string>> parse_tag(std::string_view str);
  static inline bool parse_bool(std::string_view str);
  static inline int64_t parse_int(std::string_view str);
  static inline Expected<double> parse_double_special(std::string_view str);
  static inline double parse_double(std::string_view str);

  inline void next();
  char _token;
};

}  // namespace er
