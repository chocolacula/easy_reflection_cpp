#include "parser_json.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../define_retry.h"
#include "er/reflection/reflection.h"
#include "er/types/all_types.h"
#include "er/variable/box.h"

using namespace er;

ParserJson::ParserJson(const char* input, size_t input_size)  //
    : LexerJson(input, input_size), _level(0) {
}

ParserJson::ParserJson(std::istream& stream)  //
    : LexerJson(stream), _level(0) {
}

Expected<None> ParserJson::deserialize(TypeInfo* info) {
  return parse_next(info);
}

Expected<None> ParserJson::parse(TypeInfo* info, wchar_t token) {
  switch (token) {
    case '0':
      // do nothing
      return None();
    case 't':
      return info->get<Bool>().set(true);
    case 'f':
      return info->get<Bool>().set(false);
    case 'n':
      return info->match(
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
          [this](auto&&) -> Expected<None> { return error_match(); });
    case '$':
      return info->match([this](String& s) -> Expected<None> { return s.set(get_word()); },
                         [this](Enum& e) -> Expected<None> { return e.parse(get_word()); },
                         [this](auto&&) -> Expected<None> { return error_match(); });
    case '[':
      // clang-format off
        return info->match(
            [this](Array& a) -> Expected<None> {
              return parse_array(a.nested_type(), [&](size_t i, Var var) { return add_to_array(a, i, var); });
            },
            [this](Sequence& s) -> Expected<None> {
              s.clear();
              return parse_array(s.nested_type(), [&](size_t, Var var) { return s.push(var); });
            },
            [this](Map& m) -> Expected<None> {
              return parse_map(m);
            },
            [this](auto&&) -> Expected<None> {
              return error_match();
            });
      // clang-format on
    case '{':
      return parse_object(info);
    default:
      return error_token(token);
  }
}

Expected<None> ParserJson::parse_next(TypeInfo* info) {
  return parse(info, next());
}

Expected<None> ParserJson::parse_array(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add) {
  if (++_level > kMaxLevel) {
    return error("Max depth level exceeded");
  }

  auto token = next();
  if (token == ']') {
    // an empty array
    _level -= 1;
    return None();
  }

  Box box(nested_type);
  auto boxed_info = reflection::reflect(box.var());

  for (size_t len = 0; len < kMaxArr; ++len) {
    auto ex = parse(&boxed_info, token)
                  .match_move([&, len](None&&) -> Expected<None> { return add(len, box.var()); },
                              [](Error&& err) -> Expected<None> { return err; });
    __retry(ex);

    auto token = next();
    if (token == ']') {
      --_level;
      return None();
    }
    if (token != ',') {
      return error_token(token);
    }

    // get another one
    token = next();
  }

  return None();
}

Expected<None> ParserJson::add_to_array(Array& a, size_t i, Var var) {
  if (i < a.size()) {
    auto item = a.at(i).unwrap();
    reflection::copy(item, var);
  }
  return None();
}

Expected<None> ParserJson::parse_object(TypeInfo* info) {
  if (++_level > kMaxLevel) {
    return error("Max depth level exceeded");
  }

  for (size_t len = 0; len < kMaxArr; ++len) {
    auto token = next();

    if (len == 0 && token == '}') {
      --_level;
      return None();
    }
    if (token != '$') {
      return error("Cannot reach a field name");
    }
    if (next() != ':') {
      return error("Cannot reach a field value");
    }

    auto field = info->get<Object>().get_field(get_word()).unwrap();
    __retry(parse_field(field));

    token = next();
    if (token == '}') {
      --_level;
      return None();
    }
    if (token != ',') {
      return error_token(token);
    }
  }

  return error("Max depth level exceeded");
}

Expected<None> ParserJson::parse_map(Map& map) {
  if (++_level > kMaxLevel) {
    return error("Max depth level exceeded");
  }

  map.clear();

  auto token = next();

  std::string key = "key";
  std::string val = "val";

  if (token == '$') {
    // if particular tag found parse it
    auto pos = get_word().find("!!map");
    if (pos != std::string::npos) {

      auto kv = parse_tag(get_word());
      __retry(kv);

      auto pair = kv.unwrap();
      key = std::move(pair.first);
      val = std::move(pair.second);

      // make step to get a new token
      if (next() != ',') {
        return error_token(token);
      }
      token = next();
    } else {
      return error("Cannot reach the map tag or '{'");
    }
  } else if (token != '{') {
    return error_token(token);
  }

  for (size_t len = 0; len < kMaxArr; ++len) {
    // token '{' has already been read
    ++_level;

    // parse first field key or val
    token = next();
    if (token != '$') {
      return error("Cannot reach a field name");
    }

    if (next() != ':') {
      return error("Cannot reach a field value");
    }

    Box key_box(map.key_type());
    Box val_box(map.val_type());

    if (get_word() == key) {
      __retry(parse_field(key_box.var()));
    } else if (get_word() == val) {
      __retry(parse_field(val_box.var()));
    } else {
      return Error(format("Got an unexpected field '{}' while parse map; {}", get_word(), get_position().to_string()));
    }

    token = next();
    if (token == '}') {
      --_level;
      return error("Unexpected end of JSON object");
    }
    if (token != ',') {
      return error_token(token);
    }

    // parse second field key or val
    token = next();
    if (token != '$') {
      return error("Cannot reach a field name");
    }

    if (next() != ':') {
      return error("Cannot reach a field value");
    }

    if (get_word() == key) {
      __retry(parse_field(key_box.var()));
    } else if (get_word() == val) {
      __retry(parse_field(val_box.var()));
    } else {
      return Error(format("Got an unexpected field '{}' while parse map; {}", get_word(), get_position().to_string()));
    }

    __retry(map.insert(key_box.var(), val_box.var()));

    token = next();
    if (token == '}') {
      --_level;
      token = next();
    }
    if (token == ']') {
      --_level;
      return None();
    }
    if (token != ',') {
      return error_token(token);
    }

    // take next '{'
    token = next();
  }

  return error("Max depth level exceeded");
}

wchar_t ParserJson::next() {
  return static_cast<wchar_t>(lex());
}

Error ParserJson::error(const char* str) {
  return Error(format("{}; {}", str, get_position().to_string()));
}

Error ParserJson::error_token(wchar_t token) {
  return Error(format("Unexpected token '{}'; {}", token, get_position().to_string()));
}

Error ParserJson::error_match() {
  return Error(format("Cannot match correct type; {}", get_position().to_string()));
}

Expected<None> ParserJson::parse_field(Var new_var) {
  auto info = reflection::reflect(new_var);
  return parse_next(&info);
}

Expected<std::pair<std::string, std::string>> ParserJson::parse_tag(const std::string& str) {
  auto pos1 = str.find('|');
  if (pos1 == std::string::npos) {
    return error("Cannot find '|' in the tag");
  }
  auto pos2 = str.find(':');
  if (pos2 == std::string::npos) {
    return error("Cannot find ':' in the tag");
  }

  auto key = str.substr(pos1 + 1, pos2 - (pos1 + 1));
  auto val = str.substr(pos2 + 1, str.size() - (pos2 + 1));

  return std::make_pair(std::move(key), std::move(val));
}

double ParserJson::parse_double(const std::string& str) {
  return std::strtod(&str[0], nullptr);
}
