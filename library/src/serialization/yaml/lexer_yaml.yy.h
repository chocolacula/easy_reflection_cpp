// ./../src/serialization/yaml/lexer_yaml.yy.h generated by reflex 3.0.11 from ./yaml.l

#ifndef REFLEX______SRC_SERIALIZATION_YAML_LEXER_YAML_YY_H
#define REFLEX______SRC_SERIALIZATION_YAML_LEXER_YAML_YY_H
#define IN_HEADER 1
#define REFLEX_VERSION "3.0.11"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#undef REFLEX_OPTION_dotall
#undef REFLEX_OPTION_fast
#undef REFLEX_OPTION_freespace
#undef REFLEX_OPTION_header_file
#undef REFLEX_OPTION_lex
#undef REFLEX_OPTION_lexer
#undef REFLEX_OPTION_namespace
#undef REFLEX_OPTION_noline
#undef REFLEX_OPTION_outfile
#undef REFLEX_OPTION_unicode

#define REFLEX_OPTION_dotall              true
#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         "./../src/serialization/yaml/lexer_yaml.yy.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               LexerYaml
#define REFLEX_OPTION_namespace           rf_yaml
#define REFLEX_OPTION_noline              true
#define REFLEX_OPTION_outfile             "./../src/serialization/yaml/lexer_yaml.yy.cpp"
#define REFLEX_OPTION_unicode             true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


  #include <cstdlib> // strtoul()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw
  #include <vector>   // to store YAML containers

  #include "../position.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace rf_yaml {

class LexerYaml : public reflex::AbstractLexer<reflex::Matcher> {

 public:
  LexerYaml(const char* input, size_t input_size) : LexerYaml(reflex::Input(input, input_size)){
  }

  er::Position get_position() {
    return er::Position{.column = columno(), .line_number = lineno()};
  }

  inline std::string_view get_word() {
    return _word;
  }

  // count number of newlines matched
  size_t newlines()
  {
    return chr() == '\r' ? size()/2 : size();
  }

  // parse the indent value given after a '|' or '>', if present
  void parse_indent(size_t offset)
  {
    indent = strtoul(text() + offset, NULL, 10);
  }

  // use the parsed indent value given after a '|' or '>' to adjust the indent
  void adjust_indent()
  {
    if (indent > 0)
    {
      std::vector<size_t>& stops = matcher().stops();
      size_t spaces = stops.back();
      if (spaces > indent)
      {
        stops.pop_back();
        if (stops.empty())
        {
          stops.push_back(indent);
        }
        else
        {
          spaces -= stops.back();
          stops.push_back(stops.back() + indent);
        }
        _word.append(spaces - indent, ' ');
      }
    }
  }

  // clear the string scalar before accumulating a new scalar
  void clear() {
    _word.clear();
  }

  // add one or n chars c to the string
  void add(wchar_t c, size_t n = 1) {
    while (n-- > 0) {
      _word.push_back(c);
    }
  }

  // add indent to the string, prefixed with a \n if nl is true
  void add_indent() {
    if (nl) {
      _word.push_back('\n');
    }
    size_t stop = matcher().last_stop();
    if (size() > stop) {
      _word.append(size() - stop, ' ');
    }
  }

  // if nl is true, add a \n to the string then reset nl
  void add_newline() {
    if (nl) {
      _word.push_back('\n');
      nl = false;
    }
  }

  // add sp spaces to the string
  void add_space() {
    _word.append(sp, ' ');
    sp = 0;
  }

  // chomp the string
  void chomp()
  {
    switch (mode)
    {
      case CLIP:
        while (!_word.empty() && _word.back() == '\n') {
          _word.pop_back();
        }
        _word.push_back('\n');
        break;
      case STRIP:
        while (!_word.empty() && _word.back() == '\n') {
          _word.pop_back();
        }
        break;
      case KEEP:
        break;
    }
  }

  unsigned long indent;            // block scalar indent value
  size_t sp;                       // insert spaces in folded block scalar
  bool nl;                         // insert newline in folded block scalar
  enum { CLIP, STRIP, KEEP } mode; // chomp mode

 private:
  std::string _word; // string to accumulate YAML scalars

 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  LexerYaml(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {

  indent = 0;
  nl = false;
  sp = 0;
  mode = CLIP;

  }
  static const int INITIAL = 0;
  static const int APOS = 1;
  static const int QUOT = 2;
  static const int PRES = 3;
  static const int FOLD = 4;
  static const int PBLK = 5;
  static const int FBLK = 6;
  virtual int lex(void);
  int lex(const reflex::Input& input)
  {
    in(input);
    return lex();
  }
  int lex(const reflex::Input& input, std::ostream *os)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

} // namespace rf_yaml

#endif
