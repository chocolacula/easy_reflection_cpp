// A fast JSON parser and JSON writer
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// * %o freespace to space regular expressions between concat and alternations
// * %o dotall mode: . matches \n
// * %o unicode mode: . mathes any Unicode character
// * \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character
// * JSON value types are indicated with tokens '0', 't', 'f', 'n', '$'
// * JSONParser is a recursive descent parser for JSON
// * JSONParser recurses 100 levels deep max (MAXLEVEL)
// * JSONParser accepts 1000 items per array/object max (MAXSIZE)

%option lexer=LexerJson
%option namespace=rf_json

%o fast freespace dotall unicode noline

%top{
  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw

  #include "er/serialization/parsing/position.h"
}

%class{
 public:
  LexerJson(const char* input, size_t input_size) : LexerJson(reflex::Input(input, input_size)){
  }

  rr::Position get_position() {
    return rr::Position{.column = columno(), .line_number = lineno()};
  }

  inline std::string& get_word() {
    return _word;
  }

 private:
  std::string _word; // token value for token '$' (string)
}

digit   [0-9]
digit1  [1-9]
digits  {digit}+
int     -? {digit} | -? {digit1} {digits}
frac    \. {digits}
exp     [eE] [-+]? {digits}
number  {int} {frac}? {exp}?

%x STRING

%%

[ \t\n\r]+      { /* ignore white space */ }
[\]\[}{,:]      { return text()[0]; }
null            { return '0'; }
true            { return 't'; }
false           { return 'f'; }
{number}        { _word = text(); return 'n'; }
\"              { _word.clear(); start(STRING); }

<STRING>{
\"                      { start(INITIAL); return '$'; }
\\ ["\\/]               { _word.push_back(text()[1]); }
\\ b                    { _word.push_back('\b'); }
\\ t                    { _word.push_back('\t'); }
\\ n                    { _word.push_back('\n'); }
\\ f                    { _word.push_back('\f'); }
\\ r                    { _word.push_back('\r'); }
\\ u [[:xdigit:]]{4}    { _word.push_back(strtoul(text() + 2, NULL, 16)); }
[\]-\x7f\x20-\[]        { _word.push_back(text()[0]); }
\p{Non_ASCII_Unicode}   { _word.append(str()); }
}

<*>.            { return '!'; /* error */ }

%%
