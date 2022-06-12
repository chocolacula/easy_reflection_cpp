#pragma once

#include <string>

#include "../context.h"

// clang
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

class AstCallback : public MatchFinder::MatchCallback {
 public:
  explicit AstCallback(Context* ctx) : _ctx(ctx) {
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
    // check if this class has special attribute
    if (attr == nullptr) {
      auto it = _ctx->reflect_map.find(get_offset(c, sm, opts));
      if (it == _ctx->reflect_map.end()) {
        return;
      }
      attr = &it->second;
    }

    auto name = c->getQualifiedNameAsString();

    // check if this class is already handled
    if (_ctx->result.find(name) != _ctx->result.end()) {
      return;
    }

    nlohmann::json json;
    json["id"] = 0;
    json["name"] = name;
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

        item["access"] = to_access_string(acc);
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
    _ctx->result.emplace(std::move(name), std::move(json));
  }

  inline void handle_enum(const EnumDecl* e, AttrReflect* attr, const SourceManager& sm, const LangOptions& opts) {
    // check if this enum has special attribute
    if (attr == nullptr) {
      auto it = _ctx->reflect_map.find(get_offset(e, sm, opts));
      if (it == _ctx->reflect_map.end()) {
        return;
      }
      attr = &it->second;
    }

    auto name = e->getQualifiedNameAsString();

    // check if this class is already handled
    if (_ctx->result.find(name) != _ctx->result.end()) {
      return;
    }

    nlohmann::json json;
    json["id"] = 1;
    json["name"] = name;
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
    _ctx->result.emplace(std::move(name), std::move(json));
  }

  std::string relative_path(llvm::StringRef path) {
    // filesystem::path used to get relative path for #include "<path>"
    auto fs_path = std::filesystem::path(path.str());

    // add additional step back cause in output_dir is another one directory 'reflected_types'
    std::string rel = "../";
    rel += std::filesystem::relative(fs_path, _ctx->output_dir).string();
#if defined(_WIN32)
    // use '/' instead of '\' even on Windows
    std::replace(rel.begin(), rel.end(), '\\', '/');
#endif
    return rel;
  }

  template <typename DeclT>
  static inline unsigned get_offset(const DeclT* decl, const SourceManager& sm, const LangOptions& opts) {
    Token token;
    Lexer::getRawToken(decl->getLocation(), token, sm, opts);

    return sm.getFileOffset(token.getLocation());
  }

  static inline std::string to_access_string(AccessSpecifier access) {
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
};