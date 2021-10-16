#include "parser_yaml.h"

#include <cctype>
#include <cstddef>
#include <memory>
#include <string>

#include "../define_retry.h"
#include "er/expected.h"
#include "er/reflection/reflection.h"
#include "er/types/all_types.h"
#include "er/variable/box.h"

using namespace rr;

ParserYaml::ParserYaml(const char* input, size_t input_size)  //
    : LexerYaml(input, input_size),                           //
      _token(static_cast<wchar_t>(lex())) {
}

ParserYaml::ParserYaml(std::istream& stream)  //
    : LexerYaml(stream),                      //
      _token(static_cast<wchar_t>(lex())) {
}

Expected<None> ParserYaml::deserialize(TypeInfo* info) {
  while (true) {
    if (_token == 'S') {
      next();
      continue;
    }
    if (is_end(_token)) {
      // break in the end of one document even if there are few in a file
      return Error("Unexpected end of file");
    }
    return parse(info);
  }
}

Expected<None> ParserYaml::parse(TypeInfo* info) {
  if (is_new_line(_token)) {
    next();
  }

  // just ignore tags
  if (_token == '!') {
    next();
  }

  if (_token == '*') {
    auto anchor = get_word();
    auto& box = _anchors[anchor.substr(1, anchor.size() - 1)];
    __retry(reflection::copy(info->var(), box.var()));

    next();
    if (is_new_line(_token)) {
      next();
    }
    return None();
  }

  std::string anchor;
  if (_token == '&') {
    anchor = get_word();
    next();
    if (is_new_line(_token)) {
      next();
    }
  }

  Expected<None> ex = None();
  switch (_token) {
    case '-':
      ex = parse_seq(info);
      break;
    case '>':
      ex = parse_ind(info);
      break;
    case '$':
      ex = parse_str(info);
      break;
    case '[':
      ex = info->match(
          [this](Array& a) -> Expected<None> {
            return parse_flow_seq(a.nested_type(), [&](size_t i, Var var) { return add_to_array(a, i, var); });
          },
          [this](Sequence& s) -> Expected<None> {
            return parse_flow_seq(s.nested_type(), [&](size_t, Var var) { return s.push(var); });
          },
          [this](auto&&) -> Expected<None> { return error_match(); });
      break;
    case '{':
      ex = info->match([this](Object& o) -> Expected<None> { return parse_flow_map([&]() { return add_to_obj(o); }); },
                       [this](Map& m) -> Expected<None> { return parse_flow_map(m); },
                       [this](auto&&) -> Expected<None> { return error_match(); });
      break;
    case '?':
      ex = info->match([this](Map& m) -> Expected<None> { return parse_map(m); },
                       [this](auto&&) -> Expected<None> {
                         return error("A complex key, marked '?' could be deserialized in a map key only");
                       });
      break;
    default:
      ex = error_token(_token);
      break;
  }

  if (!anchor.empty() && anchor[0] == '&') {
    auto name = anchor.substr(1, anchor.size() - 1);
    _anchors.emplace(name, Box(info->var().type()));

    reflection::copy(_anchors[name].var(), info->var());
  }

  return ex;
}

// parse string of "key: val ..."
Expected<None> ParserYaml::parse_str(TypeInfo* info) {
  // a string could be a key in a map
  // or name of a field in an object
  // or be just a string value of something
  auto ex = info->match([this](Bool& b) -> Expected<None> { return b.set(parse_bool(get_word())); },
                        [this](Integer& i) -> Expected<None> {
                          auto* p = i.var().raw_mut();
                          if (p == nullptr) {
                            return Error("Trying to set const value");
                          }

                          auto w = get_word();
                          if (w.front() == '-') {
                            auto v = std::strtoll(&w[0], nullptr, 10);
                            std::memcpy(p, &v, i.size());
                          } else {
                            auto v = std::strtoull(&w[0], nullptr, 10);
                            std::memcpy(p, &v, i.size());
                          }

                          return None();
                        },
                        [this](Floating& f) -> Expected<None> { return f.set(parse_double(get_word())); },
                        [this](String& s) -> Expected<None> { return s.set(get_word()); },
                        [this](Enum& e) -> Expected<None> { return e.parse(get_word()); },
                        [this](Map& m) -> Expected<None> { return parse_map(m); },
                        [this](Object& o) -> Expected<None> { return parse_map([&]() { return add_to_obj(o); }); },
                        [this](auto&&) -> Expected<None> { return error_match(); });
  __retry(ex);
  next();

  if (is_new_line(_token) || is_end(_token)) {
    next();
  }

  return ex;
}

// parse indented value (string, nested sequence, or nested map)
Expected<None> ParserYaml::parse_ind(TypeInfo* info) {
  next();  // skip indention token itself

  auto ex = parse(info);

  if (is_new_line(_token)) {
    next();
  }
  if (_token == '<') {
    next();
  }
  return ex;
}

// parse "- val ..."
Expected<None> ParserYaml::parse_seq(TypeInfo* info) {
  return info->match(
      [this](Array& a) -> Expected<None> {
        return parse_seq(a.nested_type(), [&](size_t i, Var var) { return add_to_array(a, i, var); });
      },
      [this](Sequence& s) -> Expected<None> {
        s.clear();
        return parse_seq(s.nested_type(), [&](size_t, Var var) { return s.push(var); });
      },
      [this](auto&& a) -> Expected<None> { return error_match(); });
}

Expected<None> ParserYaml::parse_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add) {
  if (_token == '[') {
    return parse_flow_seq(nested_type, std::move(add));
  }

  size_t level = 0;
  size_t i = 0;

  Box box(nested_type);
  auto info = reflection::reflect(box.var());

  while (!is_end(_token)) {
    if (_token == '>') {
      next();
      ++level;
    }
    while (_token == '<') {
      if (level == 0) {
        return None();
      }
      next();
      if (level == 1) {
        return None();
      }
      --level;
    }
    if (_token != '-') {
      return error_token(_token);
    }

    next();  // skip '-' itself

    __retry(parse(&info));
    __retry(add(i, box.var()));
    i++;
  }
  return None();
}

// parse "[ val, ... ]"
Expected<None> ParserYaml::parse_flow_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add) {
  next();  // skip '[' itself

  size_t level = 0;
  size_t i = 0;

  Box box(nested_type);
  auto info = reflection::reflect(box.var());

  while (!is_end(_token) && _token != 'S' && _token != ']') {
    if (is_new_line(_token)) {
      next();
    }
    if (_token == '>') {
      ++level;
      next();
    } else {
      while (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }

    __retry(parse(&info));
    i++;
    __retry(add(i, box.var()));

    if (is_new_line(_token)) {
      next();
    }
    if (_token == '>') {
      ++level;
      next();
    } else {
      while (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }
    if (_token == ',') {
      next();
    }
    if (is_new_line(_token)) {
      next();
    }
    if (_token == '>') {
      ++level;
      next();
    } else {
      while (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }
  }
  if (_token == ']') {
    next();
  }
  if (is_new_line(_token)) {
    next();
  }
  while (_token == '<' && level-- > 0) {
    next();
  }
  return None();
}

Expected<None> ParserYaml::add_to_array(Array& a, size_t i, Var var) {
  if (i < a.size()) {
    auto item = a.at(i).unwrap();
    reflection::copy(item, var);
  }
  return None();
}

Expected<None> ParserYaml::parse_map(Map& map) {
  Box key_box(map.key_type());
  Box val_box(map.val_type());

  auto info_k = reflection::reflect(key_box.var());
  auto info_v = reflection::reflect(val_box.var());

  map.clear();
  return parse_map([&]() { return add_to_map(map, &info_k, &info_v, key_box.var(), val_box.var()); });
}

Expected<None> ParserYaml::parse_map(std::function<Expected<None>()> add) {
  if (_token == '{') {
    return parse_flow_map(std::move(add));
  }

  while (_token != 'S' && !is_end(_token) && _token != '<') {

    if (is_new_line(_token)) {
      next();
    }

    __retry(add());
  }
  return None();
}

// parse "{ key:val, ... }" to an object
Expected<None> ParserYaml::parse_flow_map(std::function<Expected<None>()> add) {
  next();  // skip '{' itself

  size_t level = 0;
  while (_token != 'S' && !is_end(_token) && _token != '}') {

    if (is_new_line(_token) || _token == '$') {
      next();
    }

    if (_token == '>') {
      ++level;
      next();
    } else {
      while (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }

    if (_token == ':') {
      __retry(add());
    } else {
      return error_token(_token);
    }

    if (is_new_line(_token)) {
      next();
    }
    if (_token == '>') {
      ++level;
      next();
    } else {
      while (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }
    if (_token == ',') {
      next();
    }
    if (is_new_line(_token)) {
      next();
    }
    if (_token == '>') {
      ++level;
      next();
    } else if (_token == '<') {
      if (level > 0) {
        --level;
      }
      next();
    }
  }

  if (_token == '}') {
    next();
  }
  if (is_new_line(_token)) {
    next();
  }
  while (_token == '<' && level-- > 0) {
    next();
  }

  return None();
}

Expected<None> ParserYaml::parse_flow_map(Map& map) {
  Box box_key(map.key_type());
  Box box_val(map.val_type());

  auto info_key = reflection::reflect(box_key.var());
  auto info_val = reflection::reflect(box_val.var());

  map.clear();
  return parse_flow_map([&]() { return add_to_map(map, &info_key, &info_val, box_key.var(), box_val.var()); });
}

inline Expected<None> ParserYaml::add_to_obj(Object& obj) {
  if (_token == '$') {
    next();
  }

  if (_token != ':') {
    return error_token(_token);
  }

  auto ex = obj.get_field(get_word());
  __retry(ex);

  auto info = reflection::reflect(ex.unwrap());

  next();
  if (is_new_line(_token)) {
    next();
  }
  __retry(parse(&info));
  return None();
}

Expected<None> ParserYaml::add_to_map(Map& map, TypeInfo* info_key, TypeInfo* info_value, Var var_key, Var var_value) {
  // get a key
  if (_token == '$') {
    __retry(parse_str(info_key));
  } else if (_token == '?') {
    next();
    __retry(parse(info_key));
  } else {
    return error_token(_token);
  }

  if (_token != ':') {
    return error_token(_token);
  }
  next();

  // get a value
  __retry(parse(info_value));

  return map.insert(var_key, var_value);
}

wchar_t ParserYaml::next() {
  if (_token != 0) {
    _token = static_cast<wchar_t>(lex());
  }
  return _token;
}

bool ParserYaml::is_end(int token) {
  return token == 0 || token == 'E';
}

bool ParserYaml::is_new_line(int token) {
  return token == '=' || token == ';';
}

Error ParserYaml::error(const char* str) {
  return Error(format("{}; {}", str, get_position().to_string()));
}

Error ParserYaml::error_token(wchar_t token) {
  return Error(format("Unexpected token '{}'; {}", token, get_position().to_string()));
}

Error ParserYaml::error_match() {
  return Error(format("Cannot match correct type; {}", get_position().to_string()));
}

bool ParserYaml::parse_bool(const std::string& str) {
  if (str.size() > 5) {  // 5 is length of the longest variant 'false'
    return true;
  }

  std::string t = str;
  std::transform(t.begin(), t.end(), t.begin(), [](char c) { return std::tolower(c); });

  return !(t == "false" || t == "off" || t == "no" || t == "n");
}

double ParserYaml::parse_double(const std::string& str) {
  return std::strtod(&str[0], nullptr);
}
