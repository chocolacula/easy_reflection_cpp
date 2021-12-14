#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "llvm/ADT/StringRef.h"
#include "nlohmann/json.hpp"

// clang
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/Specifiers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Tooling/Tooling.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

class ParserCpp {
 public:
  ParserCpp(const CompilationDatabase& compdb,      //
            const std::vector<std::string>& files,  //
            std::filesystem::path&& output_dir)     //
      : _tool(compdb, files), _factory(&_ctx) {
    _ctx.output_dir = output_dir;
  }

  std::map<std::string, nlohmann::json> parse() {
    // handle macro attributes at first then
    // traverse AST, check attributes, build json objects and fill result field in context
    _tool.run(&_factory);

    return _ctx.result;
  }

 private:
  ClangTool _tool;

  struct AttrReflect {
    bool withNonPublic = false;
    bool withBase = false;
  };

  struct Context {
    std::unordered_map<unsigned, AttrReflect> reflect_map;
    std::unordered_map<unsigned, std::string> alias_map;
    std::unordered_set<unsigned> excludes;

    std::filesystem::path output_dir;
    std::map<std::string, nlohmann::json> result;
  } _ctx;

  class Matcher : public MatchFinder::MatchCallback {
   public:
    explicit Matcher(Context* ctx) : _ctx(ctx) {
    }

    void run(const MatchFinder::MatchResult& result) final {

      if (const auto* c = result.Nodes.getNodeAs<CXXRecordDecl>("c")) {
        handle_class(c, nullptr, *result.SourceManager, result.Context->getLangOpts());
      }
      if (const auto* e = result.Nodes.getNodeAs<EnumDecl>("e")) {
        handle_enum(e, nullptr, *result.SourceManager, result.Context->getLangOpts());
      }
    }

   private:
    Context* _ctx;

    void handle_class(const CXXRecordDecl* c, AttrReflect* attr, const SourceManager& sm, const LangOptions& opts) {

      if (attr == nullptr) {
        auto it = _ctx->reflect_map.find(get_offset(c, sm, opts));
        if (it == _ctx->reflect_map.end()) {
          return;
        }
        attr = &it->second;
      }

      nlohmann::json json;
      json["id"] = 0;
      json["name"] = c->getQualifiedNameAsString();
      json["origin"] = relative_path(sm.getFilename(c->getLocation()));

      if (attr->withBase) {
        auto arr_bases = nlohmann::json::array();
        for (auto&& b : c->bases()) {
          nlohmann::json item;

          item["access"] = to_access_string(b.getAccessSpecifier());
          item["name"] = b.getType()->getAsRecordDecl()->getQualifiedNameAsString();

          arr_bases.push_back(item);
        }
        json["bases"] = arr_bases;
      }

      auto arr_fields_static = nlohmann::json::array();
      auto arr_fields = nlohmann::json::array();
      for (auto&& d : c->getPrimaryContext()->decls()) {
        nlohmann::json item;

        // handle a field
        if (const auto* f = dyn_cast<FieldDecl>(d)) {
          auto offset = get_offset(f, sm, opts);

          if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
            continue;
          }

          auto acc = f->getAccess();
          if (acc != clang::AS_public && !attr->withNonPublic) {
            continue;
          }

          item["access"] = to_access_string(acc);
          item["name"] = f->getNameAsString();

          auto it = _ctx->alias_map.find(offset);
          if (it != _ctx->alias_map.end()) {
            item["alias"] = it->second;
          } else {
            item["alias"] = item["name"];
          }

          arr_fields.push_back(item);
          continue;
        }

        // handle a static field
        if (const auto* v = dyn_cast<VarDecl>(d)) {
          auto offset = get_offset(v, sm, opts);

          if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
            continue;
          }

          auto acc = v->getAccess();
          if (acc != clang::AS_public && !attr->withNonPublic) {
            continue;
          }

          item["access"] = to_access_string(acc) + " | Access::kStatic";
          item["name"] = v->getNameAsString();

          auto it = _ctx->alias_map.find(offset);
          if (it != _ctx->alias_map.end()) {
            item["alias"] = it->second;
          } else {
            item["alias"] = item["name"];
          }

          arr_fields_static.push_back(item);
          continue;
        }

        // if the declaration is nested class
        if (const auto* nc = dyn_cast<CXXRecordDecl>(d)) {
          if (nc->isThisDeclarationADefinition()) {
            handle_class(nc, attr, sm, opts);
          }
        }
        // or nested enum
        if (const auto* ne = dyn_cast<EnumDecl>(d)) {
          handle_enum(ne, attr, sm, opts);
        }
      }

      json["fields"] = arr_fields;
      json["fields_static"] = arr_fields_static;
      _ctx->result.emplace(std::make_pair(json["name"].get<std::string>(), std::move(json)));
    }

    inline void handle_enum(const EnumDecl* e, AttrReflect* attr, const SourceManager& sm, const LangOptions& opts) {

      if (attr == nullptr) {
        auto it = _ctx->reflect_map.find(get_offset(e, sm, opts));
        if (it == _ctx->reflect_map.end()) {
          return;
        }
        attr = &it->second;
      }

      nlohmann::json json;
      json["id"] = 1;
      json["name"] = e->getQualifiedNameAsString();
      json["origin"] = relative_path(sm.getFilename(e->getLocation()));

      auto arr = nlohmann::json::array();
      for (auto&& constant : e->enumerators()) {
        auto offset = get_offset(constant, sm, opts);

        if (_ctx->excludes.find(offset) != _ctx->excludes.end()) {
          continue;
        }

        nlohmann::json item;

        item["name"] = constant->getNameAsString();

        auto it = _ctx->alias_map.find(offset);
        if (it != _ctx->alias_map.end()) {
          item["alias"] = it->second;
        } else {
          item["alias"] = item["name"];
        }

        arr.push_back(item);
      }

      json["constants"] = arr;
      _ctx->result.emplace(std::make_pair(json["name"].get<std::string>(), std::move(json)));
    }

    std::string relative_path(llvm::StringRef path_ref) {
      auto path = std::filesystem::path(path_ref.str());
      // add additional step back cause in output_dir is another one directory 'reflected_types'
      return "../" + std::filesystem::relative(path, _ctx->output_dir).string();
    }

    static std::string to_access_string(AccessSpecifier access) {
      // fast convert to er/info/access.h Access enum constants string representation
      switch (access) {
        case clang::AS_public:
          return "Access::kPublic";
        case clang::AS_protected:
          return "Access::kProtected";
        case clang::AS_private:
          return "Access::kPrivate";
        default:
          return "Access::kNone";
      }
    }
  };

  class MacroCallback : public PPCallbacks {
   public:
    explicit MacroCallback(SourceManager& sm, LangOptions& opts, Context* ctx) : _sm(sm), _opts(opts), _ctx(ctx) {
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

          spelling = spell(&t, _sm);
        } while (spelling == "struct" || spelling == "class" || spelling == "enum" || spelling == "const");

        _ctx->reflect_map[_sm.getFileOffset(t.getLocation())] = attr;

      } else if (text == "ER_ALIAS") {
        std::string alias;

        // skip '('
        auto t_opt = Lexer::findNextToken(t.getLocation(), _sm, _opts);
        // get the alias token
        t_opt = Lexer::findNextToken(t_opt.getValue().getLocation(), _sm, _opts);
        t = t_opt.getValue();
        alias = spell(&t, _sm);
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
  };

  class Action : public ASTFrontendAction {
   public:
    explicit Action(Context* ctx) : _ctx(ctx), _matcher(ctx) {
      static const auto class_matcher = cxxRecordDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("c");
      _finder.addMatcher(class_matcher, &_matcher);

      static const auto enum_matcher = enumDecl(unless(isExpansionInSystemHeader())).bind("e");
      _finder.addMatcher(enum_matcher, &_matcher);
    }

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& compiler, StringRef /*in_file*/) override {
      // register macro handler
      compiler.getPreprocessor().addPPCallbacks(std::make_unique<MacroCallback>(compiler.getSourceManager(),  //
                                                                                compiler.getLangOpts(),       //
                                                                                _ctx));
      // forward work to match finder
      return _finder.newASTConsumer();
    }

   private:
    MatchFinder _finder;
    Matcher _matcher;
    Context* _ctx;
  };

  class Factory : public FrontendActionFactory {
   public:
    explicit Factory(Context* ctx) : _ctx(ctx) {
    }

    std::unique_ptr<FrontendAction> create() override {
      return std::make_unique<Action>(_ctx);
    }

   private:
    Context* _ctx;
  };

  Factory _factory;

  static inline StringRef spell(SourceRange range, const SourceManager& sm) {
    auto ch_range = CharSourceRange::getTokenRange(range);
    return Lexer::getSourceText(CharSourceRange::getTokenRange(range), sm, LangOptions(), nullptr);
  }

  static inline StringRef spell(const Token* token, const SourceManager& sm) {
    SourceLocation b;
    SourceLocation e;

    if (tok::isStringLiteral(token->getKind())) {
      b = token->getLocation().getLocWithOffset(1);
      e = token->getEndLoc().getLocWithOffset(-2);
    } else {
      b = token->getLocation();
      e = token->getEndLoc().getLocWithOffset(-1);
    }

    return spell(SourceRange(b, e), sm);
  }

  template <typename DeclT>
  static inline unsigned get_offset(const DeclT* decl, const SourceManager& sm, const LangOptions& opts) {
    Token token;
    Lexer::getRawToken(decl->getLocation(), token, sm, opts);

    return sm.getFileOffset(token.getLocation());
  }
};