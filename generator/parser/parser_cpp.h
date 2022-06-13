#pragma once

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "action.h"

// clang
#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ADT/StringRef.h"

class ParserCpp {
 public:
  // may throw an exception
  ParserCpp(std::string_view compdb_dir,                  //
            const std::vector<std::string>& input_files,  //
            std::string_view output_dir)                  //
      : _compdb(load_compdb(compdb_dir)),                 //
        _tool(*_compdb, input_files),
        _factory(&_ctx) {

    _ctx.output_dir = output_dir;
  }

  std::unordered_map<std::string, nlohmann::json> parse() {
    // handle macro attributes at first then
    // traverse AST, check attributes, build json objects and fill result field in context
    _tool.run(&_factory);

    return _ctx.result;
  }

 private:
  std::unique_ptr<tooling::CompilationDatabase> _compdb;
  tooling::ClangTool _tool;
  ActionFactory _factory;
  Context _ctx;

  static std::unique_ptr<tooling::CompilationDatabase> load_compdb(std::string_view compdb_dir) {
    std::string err;

    auto source = StringRef(compdb_dir.data(), compdb_dir.size());  // MSVC requires
    auto compdb = tooling::CompilationDatabase::autoDetectFromDirectory(source, err);

    if (compdb == nullptr) {
#if _WIN32
      std::cerr << "Cannot find compilation database, aborted" << std::endl;
#endif
      throw std::runtime_error("Cannot find compilation database, aborted");
    }
    return compdb;
  }
};