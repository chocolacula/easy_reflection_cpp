#include "parser_cpp.h"

#include <cstddef>
#include <stdexcept>

#include "action.h"

// clang
#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ADT/StringRef.h"

namespace {
std::unique_ptr<tooling::CompilationDatabase> load_compdb(std::string_view compdb_dir) {
  std::string err;

  auto source = StringRef(compdb_dir.data(), compdb_dir.size());
  auto compdb = tooling::CompilationDatabase::autoDetectFromDirectory(source, err);

  if (compdb == nullptr) {
    throw std::runtime_error("Cannot find compilation database, aborted");
  }
  return compdb;
}
}  // namespace

ParserCpp::ParserCpp(std::string_view compdb_dir,  //
                     std::string_view output_dir)  //
    : _compdb(load_compdb(compdb_dir)) {
  _ctx.output_dir = output_dir;
}

std::unordered_map<std::string, nlohmann::json> ParserCpp::parse(const std::vector<std::string>& input_files) {
  tooling::ClangTool tool(*_compdb, input_files);
  ActionFactory factory(&_ctx);

  // handle macro attributes at first then
  // traverse AST, check attributes, build json objects and fill result field in context
  tool.run(&factory);

  return _ctx.result;
}

ParserCpp::~ParserCpp() = default;
