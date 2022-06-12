#pragma once

#include "../context.h"

// clang
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class MacroCallback : public PPCallbacks {
 public:
  explicit MacroCallback(SourceManager& sm, LangOptions& opts, Context* ctx)  //
      : _sm(sm), _opts(opts), _ctx(ctx) {
  }

  void MacroExpands(const Token& token, const MacroDefinition& /*def*/, SourceRange /*range*/,
                    const MacroArgs* /*args*/) final {

    if (_sm.isInSystemHeader(token.getLocation())) {
      return;
    }

    auto text = spell(&token, _sm);
    auto t = token;

    if (text == "ER_REFLECT") {
      AttrReflect attr;

      while (true) {
        auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        t = t_opt.getValue();

        if (t.getKind() == tok::r_paren) {
          break;
        }
        if (!(t.getKind() == tok::raw_identifier || t.getKind() == tok::string_literal)) {
          continue;
        }

        auto spelling = spell(&t, _sm);
        if (spelling == "WithNonPublic") {
          attr.withNonPublic = true;
        } else if (spelling == "WithBase") {
          attr.withBase = true;
        }
      }

      std::string spelling;
      do {
        // skip ')' and keywords 'struct' 'class' or 'enum'
        auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        t = t_opt.getValue();

        spelling = spell(&t, _sm).str();
      } while (spelling == "struct" || spelling == "class" || spelling == "enum" || spelling == "const");

      _ctx->reflect_map[_sm.getFileOffset(t.getLocation())] = attr;

    } else if (text == "ER_ALIAS") {
      std::string alias;

      // skip '('
      auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
      // get the alias token
      t_opt = Lexer::findNextToken(t_opt.getValue().getLocation(), _sm, _opts);
      t = t_opt.getValue();
      alias = spell(&t, _sm).str();
      // skip ')'
      t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
      t = t_opt.getValue();

      auto t_origin = t;
      while (true) {
        // skip all type parts
        t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        t = t_opt.getValue();

        auto k = t.getKind();
        if (k == tok::coloncolon || k == tok::less || k == tok::greater) {
          continue;
        }

        if (k == tok::raw_identifier) {
          t_origin = t;
        } else {
          break;
        }
      }

      _ctx->alias_map[_sm.getFileOffset(t_origin.getLocation())] = alias;

    } else if (text == "ER_EXCLUDE") {

      auto t_origin = t;
      while (true) {
        // skip all type parts
        auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        t = t_opt.getValue();

        auto k = t.getKind();
        if (k == tok::coloncolon || k == tok::less || k == tok::greater) {
          continue;
        }

        if (k == tok::raw_identifier) {
          t_origin = t;
        } else {
          break;
        }
      }

      _ctx->excludes.insert(_sm.getFileOffset(t_origin.getLocation()));
    }
  }

 private:
  SourceManager& _sm;
  LangOptions& _opts;
  Context* _ctx;

  static inline StringRef spell(SourceRange range, const SourceManager& sm) {
    return Lexer::getSourceText(CharSourceRange::getTokenRange(range), sm, LangOptions(), nullptr);
  }

  static inline StringRef spell(const Token* token, const SourceManager& sm) {
    SourceLocation begin;
    SourceLocation end;

    if (tok::isStringLiteral(token->getKind())) {
      begin = token->getLocation().getLocWithOffset(1);
      end = token->getEndLoc().getLocWithOffset(-2);
    } else {
      begin = token->getLocation();
      end = token->getEndLoc().getLocWithOffset(-1);
    }

    return spell(SourceRange(begin, end), sm);
  }
};