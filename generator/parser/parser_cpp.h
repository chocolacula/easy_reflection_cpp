#pragma once

#include <string_view>
#include <vector>

#include "context.h"

namespace clang {
namespace tooling {
class CompilationDatabase;
}  // namespace tooling
}  // namespace clang

class ParserCpp {
 public:
  // may throw an exception
  ParserCpp(std::string_view compdb_dir, std::string_view output_dir);
  ~ParserCpp();

  std::unordered_map<std::string, nlohmann::json> parse(const std::vector<std::string>& input_files);

 private:
  std::unique_ptr<clang::tooling::CompilationDatabase> _compdb;
  Context _ctx;
};
