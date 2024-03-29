// library/lexers/../src/serialization/json/lexer_json.yy.cpp generated by reflex 3.0.11 from library/lexers/json.l

#define REFLEX_VERSION "3.0.11"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#undef REFLEX_OPTION_fast
#undef REFLEX_OPTION_freespace
#undef REFLEX_OPTION_header_file
#undef REFLEX_OPTION_lex
#undef REFLEX_OPTION_lexer
#undef REFLEX_OPTION_namespace
#undef REFLEX_OPTION_noindent
#undef REFLEX_OPTION_noline
#undef REFLEX_OPTION_outfile
#undef REFLEX_OPTION_unicode

#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         "library/lexers/../src/serialization/json/lexer_json.yy.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               LexerJson
#define REFLEX_OPTION_namespace           rf_json
#define REFLEX_OPTION_noindent            true
#define REFLEX_OPTION_noline              true
#define REFLEX_OPTION_outfile             "library/lexers/../src/serialization/json/lexer_json.yy.cpp"
#define REFLEX_OPTION_unicode             true

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


  #include <cstdlib>  // strtoul(), strtod()
  #include <iostream> // std::cout etc.
  #include <iomanip>  // std::setw

  #include "../position.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define WITH_NO_INDENT
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

namespace rf_json {

class LexerJson : public reflex::AbstractLexer<reflex::Matcher> {

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

 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  LexerJson(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int STRING = 1;
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

} // namespace rf_json

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// A fast JSON parser
// Written by Robert van Engelen
// Edited by Maxim Voloshin
//
// * %o unicode mode: . mathes any Unicode character
// * \p{Non_ASCII_Unicode} matches any non-ASCII Unicode character


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace rf_json {
extern void reflex_code_INITIAL(reflex::Matcher&);
} // namespace rf_json
namespace rf_json {
extern void reflex_code_STRING(reflex::Matcher&);
} // namespace rf_json

int rf_json::LexerJson::lex(void)
{
  static const reflex::Pattern PATTERN_INITIAL(reflex_code_INITIAL);
  static const reflex::Pattern PATTERN_STRING(reflex_code_STRING);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule library/lexers/json.l:44: [ \t\n\r]+ :
{ /* ignore white space */ }
            break;
          case 2: // rule library/lexers/json.l:45: [\]\[}{,:] :
{ return text()[0]; }
            break;
          case 3: // rule library/lexers/json.l:46: {scalar} :
{ _word = text(); return 's'; }
            break;
          case 4: // rule library/lexers/json.l:47: \" :
{ _word.clear(); start(STRING); }

            break;
        }
        break;
      case STRING:
        matcher().pattern(PATTERN_STRING);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule library/lexers/json.l:50: \" :
{ start(INITIAL); return '$'; }
            break;
          case 2: // rule library/lexers/json.l:51: \\ ["\\/] :
{ _word.push_back(text()[1]); }
            break;
          case 3: // rule library/lexers/json.l:52: [^"\\]*(?:\\.[^"\\]*)* :
{ _word = str(); }
            break;
        }
        break;
      default:
        start(0);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  TABLES                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

namespace rf_json {

void reflex_code_INITIAL(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  c1 = m.FSM_CHAR();
  if (c1 == '}') goto S19;
  if (c1 == '{') goto S19;
  if ('r' <= c1 && c1 <= 'u') goto S28;
  if (c1 == 'n') goto S28;
  if (c1 == 'l') goto S28;
  if ('e' <= c1 && c1 <= 'f') goto S28;
  if (c1 == 'a') goto S28;
  if (c1 == ']') goto S19;
  if (c1 == '[') goto S19;
  if (c1 == 'E') goto S28;
  if (c1 == ':') goto S19;
  if ('0' <= c1 && c1 <= '9') goto S28;
  if (c1 == ',') goto S19;
  if ('+' <= c1 && c1 <= '.') goto S28;
  if (c1 == '"') goto S21;
  if (c1 == ' ') goto S23;
  if (c1 == '\r') goto S23;
  if ('\t' <= c1 && c1 <= '\n') goto S23;
  return m.FSM_HALT(c1);

S19:
  m.FSM_TAKE(2);
  return m.FSM_HALT();

S21:
  m.FSM_TAKE(4);
  return m.FSM_HALT();

S23:
  m.FSM_TAKE(1);
  c1 = m.FSM_CHAR();
  if (c1 == ' ') goto S23;
  if (c1 == '\r') goto S23;
  if ('\t' <= c1 && c1 <= '\n') goto S23;
  return m.FSM_HALT(c1);

S28:
  m.FSM_HEAD(0);
  c1 = m.FSM_CHAR();
  if (c1 == '}') goto S42;
  if ('r' <= c1 && c1 <= 'u') goto S28;
  if (c1 == 'n') goto S28;
  if (c1 == 'l') goto S28;
  if ('e' <= c1 && c1 <= 'f') goto S28;
  if (c1 == 'a') goto S28;
  if (c1 == ']') goto S42;
  if (c1 == 'E') goto S28;
  if ('0' <= c1 && c1 <= '9') goto S28;
  if (c1 == ',') goto S42;
  if ('+' <= c1 && c1 <= '.') goto S28;
  if (c1 == '\n') goto S42;
  return m.FSM_HALT(c1);

S42:
  m.FSM_TAKE(3);
  m.FSM_TAIL(0);
  return m.FSM_HALT();
}

} // namespace rf_json

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

namespace rf_json {

void reflex_code_STRING(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  m.FSM_TAKE(3);
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S48;
  if (241 <= c1 && c1 <= 243) goto S46;
  if (c1 == 240) goto S44;
  if (238 <= c1 && c1 <= 239) goto S42;
  if (c1 == 237) goto S40;
  if (225 <= c1 && c1 <= 236) goto S38;
  if (c1 == 224) goto S36;
  if (194 <= c1 && c1 <= 223) goto S34;
  if (128 <= c1) return m.FSM_HALT(c1);
  if (c1 == '\\') goto S15;
  if (c1 == '"') goto S13;
  if (0 <= c1 && c1 <= 127) goto S21;
  return m.FSM_HALT(c1);

S13:
  m.FSM_TAKE(1);
  return m.FSM_HALT();

S15:
  c1 = m.FSM_CHAR();
  if (c1 == '\\') goto S50;
  if (c1 == '/') goto S50;
  if (c1 == '"') goto S50;
  if ('\v' <= c1) goto S64;
  if ('\n' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '\t') goto S64;
  return m.FSM_HALT(c1);

S21:
  m.FSM_TAKE(3);
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S48;
  if (241 <= c1 && c1 <= 243) goto S46;
  if (c1 == 240) goto S44;
  if (238 <= c1 && c1 <= 239) goto S42;
  if (c1 == 237) goto S40;
  if (225 <= c1 && c1 <= 236) goto S38;
  if (c1 == 224) goto S36;
  if (194 <= c1 && c1 <= 223) goto S34;
  if (c1 == '\\') goto S78;
  if ('#' <= c1 && c1 <= 127) goto S21;
  if ('"' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '!') goto S21;
  return m.FSM_HALT(c1);

S34:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S36:
  c1 = m.FSM_CHAR();
  if (160 <= c1 && c1 <= 191) goto S81;
  return m.FSM_HALT(c1);

S38:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S83;
  return m.FSM_HALT(c1);

S40:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 159) goto S85;
  return m.FSM_HALT(c1);

S42:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S87;
  return m.FSM_HALT(c1);

S44:
  c1 = m.FSM_CHAR();
  if (144 <= c1 && c1 <= 191) goto S89;
  return m.FSM_HALT(c1);

S46:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S91;
  return m.FSM_HALT(c1);

S48:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 143) goto S93;
  return m.FSM_HALT(c1);

S50:
  m.FSM_TAKE(2);
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S122;
  if (241 <= c1 && c1 <= 243) goto S120;
  if (c1 == 240) goto S118;
  if (238 <= c1 && c1 <= 239) goto S116;
  if (c1 == 237) goto S114;
  if (225 <= c1 && c1 <= 236) goto S112;
  if (c1 == 224) goto S110;
  if (194 <= c1 && c1 <= 223) goto S108;
  if (128 <= c1 && c1 <= 191) goto S64;
  if (c1 == '\\') goto S78;
  if ('#' <= c1 && c1 <= 127) goto S95;
  if ('"' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '!') goto S95;
  return m.FSM_HALT(c1);

S64:
  m.FSM_TAKE(3);
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S122;
  if (241 <= c1 && c1 <= 243) goto S120;
  if (c1 == 240) goto S118;
  if (238 <= c1 && c1 <= 239) goto S116;
  if (c1 == 237) goto S114;
  if (225 <= c1 && c1 <= 236) goto S112;
  if (c1 == 224) goto S110;
  if (194 <= c1 && c1 <= 223) goto S108;
  if (128 <= c1 && c1 <= 191) goto S64;
  if (c1 == '\\') goto S78;
  if ('#' <= c1 && c1 <= 127) goto S95;
  if ('"' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '!') goto S95;
  return m.FSM_HALT(c1);

S78:
  c1 = m.FSM_CHAR();
  if ('\v' <= c1) goto S64;
  if ('\n' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '\t') goto S64;
  return m.FSM_HALT(c1);

S81:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S83:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S85:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S87:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S89:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S124;
  return m.FSM_HALT(c1);

S91:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S126;
  return m.FSM_HALT(c1);

S93:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S128;
  return m.FSM_HALT(c1);

S95:
  m.FSM_TAKE(3);
  c1 = m.FSM_CHAR();
  if (c1 == 244) goto S122;
  if (241 <= c1 && c1 <= 243) goto S120;
  if (c1 == 240) goto S118;
  if (238 <= c1 && c1 <= 239) goto S116;
  if (c1 == 237) goto S114;
  if (225 <= c1 && c1 <= 236) goto S112;
  if (c1 == 224) goto S110;
  if (194 <= c1 && c1 <= 223) goto S108;
  if (c1 == '\\') goto S78;
  if ('#' <= c1 && c1 <= 127) goto S95;
  if ('"' <= c1) return m.FSM_HALT(c1);
  if (0 <= c1 && c1 <= '!') goto S95;
  return m.FSM_HALT(c1);

S108:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S110:
  c1 = m.FSM_CHAR();
  if (160 <= c1 && c1 <= 191) goto S130;
  return m.FSM_HALT(c1);

S112:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S132;
  return m.FSM_HALT(c1);

S114:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 159) goto S134;
  return m.FSM_HALT(c1);

S116:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S136;
  return m.FSM_HALT(c1);

S118:
  c1 = m.FSM_CHAR();
  if (144 <= c1 && c1 <= 191) goto S138;
  return m.FSM_HALT(c1);

S120:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S140;
  return m.FSM_HALT(c1);

S122:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 143) goto S142;
  return m.FSM_HALT(c1);

S124:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S126:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S128:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S21;
  return m.FSM_HALT(c1);

S130:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S132:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S134:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S136:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S138:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S144;
  return m.FSM_HALT(c1);

S140:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S146;
  return m.FSM_HALT(c1);

S142:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S148;
  return m.FSM_HALT(c1);

S144:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S146:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);

S148:
  c1 = m.FSM_CHAR();
  if (128 <= c1 && c1 <= 191) goto S95;
  return m.FSM_HALT(c1);
}

} // namespace rf_json

