#pragma once

#include <unistd.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "config.h"

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

  void correct_config(Config* config) {
    correct_path(&(config->compdb_dir));
    correct_path(&(config->templates.for_enum));
    correct_path(&(config->templates.object));
    correct_path(&(config->output_dir));

    complete_files(&(config->input));
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

  void correct_path(std::string* path) {
    if (path->front() != '/') {
      path->insert(0, _root);
    }

    if (std::filesystem::is_directory(*path)) {
      *path += '/';
    }
  }

  void complete_files(std::vector<std::string>* pathes) {
    auto old = *pathes;
    pathes->clear();

    for (auto path : old) {
      correct_path(&path);
      std::filesystem::path fs_p = path;

      if (std::filesystem::is_directory(fs_p)) {
        for (auto&& file_path : std::filesystem::recursive_directory_iterator(fs_p)) {
          pathes->push_back(file_path.path().string());
        }
      } else {
        pathes->push_back(path);
      }
    }
  }
};
