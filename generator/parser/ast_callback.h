#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "context.h"

// clang
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

struct JsonBuilder {
  JsonBuilder(Context* ctx,  //
              const SourceManager* sm, const LangOptions& opts)
      : _ctx(ctx), _sm(sm), _opts(opts) {
  }

  void handle_class(const CXXRecordDecl* c) {
    if (!c->hasAttr<ErReflectAttr>()) {
      return;
    }
    _options = get_options(c);

    add_class(c);
  }

  void handle_enum(const EnumDecl* e) {
    if (!e->hasAttr<ErReflectAttr>()) {
      return;
    }
    _options = get_options(e);

    add_enum(e);
  }

 private:
  Context* _ctx;
  std::unordered_set<ErReflectAttr::Option> _options;

  const SourceManager* _sm;
  const LangOptions& _opts;

  void add_class(const CXXRecordDecl* c) {
    auto name = c->getQualifiedNameAsString();

    // check if this class is already handled
    if (_ctx->result.count(name) != 0) {
      return;
    }

    nlohmann::json json;

    json["kind"] = 0;
    json["name"] = name;
    json["origin"] = file_name(c);

    if (_options.count(ErReflectAttr::Option::Base) != 0) {
      auto& arr_bases = json["bases"];

      for (auto&& b : c->bases()) {
        nlohmann::json item;

        item["access"] = JsonBuilder::access_str(b.getAccessSpecifier());
        item["name"] = b.getType()->getAsRecordDecl()->getQualifiedNameAsString();

        arr_bases.push_back(item);
      }
    }

    auto& fields_static = json["fields_static"];
    auto& fields = json["fields"];
    auto& func_static = json["func_static"];
    auto& func = json["func"];

    for (auto&& d : c->getPrimaryContext()->decls()) {

      if (const auto* f = dyn_cast<FieldDecl>(d)) {

        add_field(&fields, f);

      } else if (const auto* v = dyn_cast<VarDecl>(d)) {

        add_field(&fields_static, v);

      } else if (const auto* f = dyn_cast<FunctionDecl>(d)) {
        if (f->isStatic()) {

          add_function(&func_static, f, name);

        } else {

          add_function(&func, f, name);
        }
      } else if (const auto* nc = dyn_cast<CXXRecordDecl>(d)) {
        if (!nc->isThisDeclarationADefinition() ||  //
            nc->hasAttr<ErReflectAttr>()) {
          // skip nested classes with dedicated 'reflect' attribute,
          // handle them further as root declaration
          continue;
        }
        add_class(nc);

      } else if (const auto* ne = dyn_cast<EnumDecl>(d)) {
        if (ne->hasAttr<ErReflectAttr>()) {
          // skip nested enums with dedicated 'reflect' attribute,
          // handle them further as root declaration
          continue;
        }
        add_enum(ne);
      }
    }

    _ctx->result.emplace(std::move(name), std::move(json));
  }

  void add_enum(const EnumDecl* e) {
    auto name = e->getQualifiedNameAsString();

    // check if this enum is already handled
    if (_ctx->result.find(name) != _ctx->result.end()) {
      return;
    }

    nlohmann::json json;
    json["kind"] = 1;
    json["name"] = name;
    json["origin"] = file_name(e);

    auto& arr = json["constants"];

    for (auto&& c : e->enumerators()) {
      if (c->hasAttr<ErExcludeAttr>()) {
        continue;
      }
      auto& item = arr.emplace_back();
      set_name(&item, c);
    }

    _ctx->result.emplace(std::move(name), std::move(json));
  }

  void add_function(nlohmann::json* functions, const FunctionDecl* f, const std::string& class_name) {
    if (f->hasAttr<ErExcludeAttr>()) {
      return;
    }

    auto acc = f->getAccess();
    if ((acc != clang::AS_public && _options.count(ErReflectAttr::Option::NonPublic) == 0) ||
        _options.count(ErReflectAttr::Option::Func) == 0) {
      return;
    }

    auto name = f->getNameAsString();

    if (name == class_name ||                                 // constructor
        (name.find(class_name, 1) == 1 && name[0] == '~') ||  // destructor
        name.find("operator") == 0) {
      return;
    }

    auto& func = functions->emplace_back();

    set_name(&func, f);
    func["access"] = access_str(acc);
    func["return"] = f->getDeclaredReturnType().getAsString();

    auto& params = func["params"];
    for (auto&& p : f->parameters()) {
      params.emplace_back(type_str(p));
    }
  }

  void add_field(nlohmann::json* fields, const DeclaratorDecl* f) {
    if (f->hasAttr<ErExcludeAttr>()) {
      return;
    }

    auto acc = f->getAccess();
    if ((acc != clang::AS_public && _options.count(ErReflectAttr::Option::NonPublic) == 0) ||
        _options.count(ErReflectAttr::Option::Data) == 0) {
      return;
    }

    auto& field = fields->emplace_back();

    set_name(&field, f);
    field["access"] = access_str(acc);
    field["type"] = type_str(f);
  }

  inline std::string type_str(const DeclaratorDecl* decl) const {
    const PrintingPolicy pp(_opts);
    return QualType::getAsString(decl->getType().split(), pp);
  }

  std::string file_name(const NamedDecl* decl) const {
    auto path = _sm->getFilename(decl->getLocation());

    // filesystem::path used to get relative path for #include "<path>"
    auto fs_path = std::filesystem::path(path.begin(), path.end());

    // add additional step back cause in output_dir is another one directory 'reflected_types'
    std::string rel = "../";
    rel += std::filesystem::relative(fs_path, _ctx->output_dir).string();
#if defined(_WIN32)
    // use '/' instead of '\' even on Windows
    std::replace(rel.begin(), rel.end(), '\\', '/');
#endif
    return rel;
  }

  static inline std::string access_str(AccessSpecifier access) {
    // fast convert to er/info/access.h Access enum constants string representation
    switch (access) {
      case clang::AS_public:
        return "kPublic";
      case clang::AS_protected:
        return "kProtected";
      case clang::AS_private:
        return "kPrivate";
      default:
        return "kNone";
    }
  }

  static void set_name(nlohmann::json* item, const NamedDecl* decl) {
    auto name = decl->getNameAsString();

    (*item)["name"] = name;

    if (const auto* alias = decl->getAttr<ErAliasAttr>()) {
      (*item)["alias"] = alias->getName().str();

      return;
    }
    (*item)["alias"] = name;
  }

  static std::unordered_set<ErReflectAttr::Option> get_options(const NamedDecl* decl) {
    std::unordered_set<ErReflectAttr::Option> opts;

    if (const auto* r = decl->getAttr<ErReflectAttr>()) {
      for (auto&& option : r->options()) {
        opts.insert(option);
      }

      // empty options == All
      if (opts.empty() || opts.count(ErReflectAttr::Option::All) != 0) {
        opts.insert(ErReflectAttr::Option::Base);
        opts.insert(ErReflectAttr::Option::NonPublic);
        opts.insert(ErReflectAttr::Option::Data);
        opts.insert(ErReflectAttr::Option::Func);
      }
    }
    return opts;
  }
};

class AstCallback : public MatchFinder::MatchCallback {
 public:
  explicit AstCallback(Context* ctx) : _ctx(ctx) {
  }

  void run(const MatchFinder::MatchResult& result) final {
    JsonBuilder builder(_ctx, result.SourceManager, result.Context->getLangOpts());

    if (const auto* c = result.Nodes.getNodeAs<CXXRecordDecl>("c")) {
      builder.handle_class(c);

    } else if (const auto* e = result.Nodes.getNodeAs<EnumDecl>("e")) {
      builder.handle_enum(e);

    } else {
      throw std::runtime_error("Unrechable case");
    }
  }

 private:
  Context* _ctx;
};
