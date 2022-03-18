// A fast JSON parser
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// * %o unicode mode: . mathes any Unicode character
// * \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character

%option lexer=LexerJson
%option namespace=rf_json

%o fast freespace unicode noline noindent

%top{
  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw

  #include "../position.h"
}

%class{
 public:
  LexerJson(const char* input, size_t input_size) : LexerJson(reflex::Input(input, input_size)){
  }

  er::Position get_position() {
    return er::Position{columno(), lineno()};
  }

  inline std::string& get_word() {
    return _word;
  }

 private:
  std::string _word; // token value
}

scalar  [0-9aeEflnrstu\.\+\-]+(?=[,}\]\n])

%x STRING

%%

[ \t\n\r]+      { /* ignore white space */ }
[\]\[}{,:]      { return text()[0]; }
{scalar}        { _word = text(); return 's'; }
\"              { _word.clear(); start(STRING); }

<STRING>{
\"                     { start(INITIAL); return '$'; }
\\ ["\\/]              { _word.push_back(text()[1]); }
[^"\\]*(?:\\.[^"\\]*)* { _word = str(); }
}

%%
