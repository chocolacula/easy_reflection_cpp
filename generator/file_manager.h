#pragma once

#include <unistd.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#if defined(__linux__)
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

class FileManager {
 public:
  FileManager() {
#if defined(__linux__)
    auto raw_path = std::make_unique<char[]>(PATH_MAX);
    auto size = readlink("/proc/self/exe", raw_path.get(), PATH_MAX);
    if (size == -1) {
      size = 0;
    }
#elif defined(__APPLE__)
    // get size first
    uint32_t size = 0;
    auto code = _NSGetExecutablePath(nullptr, &size);

    // get the path
    auto raw_path = std::make_unique<char[]>(size);
    code = _NSGetExecutablePath(raw_path.get(), &size);
    if (code == -1) {
      size = 0;
    }
#endif

    _root = std::string(raw_path.get(), size);
    cut_filename(&_root);
  }

  const std::string& root() {
    return _root;
  }

  std::string correct_path(const std::string& path) {
    if (path[0] == '/') {
      return std::string(path);
    }

    auto t = _root;
    t += path;
    return t;
  }

  std::ofstream create_reflection_header(const std::string& dir) {
    std::ofstream header;
    header.open(correct_path(dir) + "/reflection.h");

    header << R"(#pragma once

#include "er/reflection/reflection.h"
#include "er/types/all_types.h"

// generated
)";

    return header;
  }

  std::vector<std::string> find_files(const std::vector<std::string>& pathes) {
    std::vector<std::string> result;

    for (auto&& p : pathes) {
      auto path = correct_path(p);

      if (is_dir(path)) {
        for (auto&& file_path : std::filesystem::recursive_directory_iterator(path)) {
          result.push_back(file_path.path().string());
        }
      } else {
        result.push_back(path);
      }
    }

    return result;
  }

  static bool is_dir(const std::string& path) {
    return path.back() == '/';
  }

 private:
  std::string _root;

  static void cut_filename(std::string* str) {
    auto pos = str->find_last_of('/');

    if (pos != std::string::npos) {
      pos += 1;
      str->erase(pos, str->length() - pos);
    }
  }
};

std::string clear_name(const std::string& qualified_name) {
  auto pos = qualified_name.find_last_of(':');

  if (pos != std::string::npos) {
    pos += 1;
    return qualified_name.substr(pos, qualified_name.length() - pos);
  }

  return qualified_name;
}