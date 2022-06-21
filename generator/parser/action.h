#pragma once

#include "callbacks/ast_callback.h"
#include "callbacks/macro_callback.h"

// clang
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"

class Action : public ASTFrontendAction {
 public:
  explicit Action(Context* ctx) : _ast_callback(ctx), _ctx(ctx) {
    static const auto class_matcher = cxxRecordDecl(isDefinition(), unless(isExpansionInSystemHeader())).bind("c");
    _finder.addMatcher(class_matcher, &_ast_callback);
    static const auto enum_matcher = enumDecl(unless(isExpansionInSystemHeader())).bind("e");
    _finder.addMatcher(enum_matcher, &_ast_callback);
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& compiler, StringRef /*in_file*/)
  override {
    // register macro handler
    compiler.getPreprocessor().addPPCallbacks(std::make_unique<MacroCallback>(compiler.getSourceManager(),  //
                                                                              compiler.getLangOpts(),       //
                                                                              _ctx));
    // forward work to match finder
    return _finder.newASTConsumer();
  }

 private:
  MatchFinder _finder;
  AstCallback _ast_callback;
  Context* _ctx;
};

class ActionFactory : public tooling::FrontendActionFactory {
 public:
  explicit ActionFactory(Context* ctx) : _ctx(ctx) {
  }

  std::unique_ptr<FrontendAction> create() override {
    return std::make_unique<Action>(_ctx);
  }

 private:
  Context* _ctx;
};