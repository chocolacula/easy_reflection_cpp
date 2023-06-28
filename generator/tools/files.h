#pragma once

#include <string_view>
#include <vector>

#include "../config.h"

class Files {
 public:
  Files();

  const std::string& root();

  void correct_config(Config* config);

#if defined(_WIN32)
  static std::string to_utf8(const wchar_t* str, size_t size);
  static std::wstring from_utf8(const char* str, size_t size);
#endif

 private:
  std::string _root;

  // Use '/' as path separator even on Windows
  static constexpr char kDelim = '/';

  static inline std::string executable_name();
  void correct_path(std::string* path);

  // change path to a directory to paths to files inside
  void complete_files(std::vector<std::string>* paths);

  static void cut_filename(std::string* str);
  static inline bool is_absolute(const std::string& path);
};
