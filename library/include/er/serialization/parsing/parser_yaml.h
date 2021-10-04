#pragma once

#include <memory>

#include "er/info/type_info/variants/array/array.h"
#include "er/info/type_info/variants/map/map.h"
#include "er/info/type_info/variants/object.h"
#include "er/variable/var.h"

namespace rf_yaml {
class LexerYaml;
}

namespace rr {

struct TypeInfo;

class ParserYaml {
 public:
  ParserYaml(const char* input, size_t input_size);
  explicit ParserYaml(std::istream& stream);
  ~ParserYaml();

  Expected<None> deserialize(TypeInfo* info);

 private:
  Expected<None> parse(TypeInfo* info);
  Expected<None> parse_str(TypeInfo* info);
  Expected<None> parse_ind(TypeInfo* info);

  Expected<None> parse_seq(TypeInfo* info);
  Expected<None> parse_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add);
  Expected<None> parse_flow_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add);

  static inline Expected<None> add_to_array(Array& a, size_t i, Var var);

  Expected<None> parse_map(Map& map);
  Expected<None> parse_map(std::function<Expected<None>()> add);
  Expected<None> parse_flow_map(std::function<Expected<None>()> add);
  inline Expected<None> parse_flow_map(Map& map);

  inline Expected<None> add_to_obj(Object& obj);
  inline Expected<None> add_to_map(Map& map, TypeInfo* info_key, TypeInfo* info_value, Var var_key, Var var_value);

  wchar_t next();

  static inline bool is_end(int token);
  static inline bool is_new_line(int token);

  inline Error error(const char* str);
  inline Error error_token(wchar_t token);
  inline Error error_match();

  static inline bool parse_bool(const std::string& str);
  static inline int64_t parse_int(const std::string& str);
  static inline double parse_double(const std::string& str);

  std::unique_ptr<rf_yaml::LexerYaml> _lexer;
  wchar_t _token;
  std::unordered_map<std::string, Var> _anchors;
};

}  // namespace rr

#undef __retry