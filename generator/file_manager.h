#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "config.h"
#include "executable_name.h"

class FileManager {
 public:
  FileManager() {
    _root = executable_name();
    cut_filename(&_root);
  }

  const std::string& root() {
    return _root;
  }

  void correct_config(Config* config) {
    correct_path(&(config->compdb_dir));
    correct_path(&(config->templates.header));
    correct_path(&(config->templates.for_enum));
    correct_path(&(config->templates.object));
    correct_path(&(config->output_dir));

    complete_files(&(config->input));
  }

 private:
  std::string _root;

  static constexpr char _delim = std::filesystem::path::preferred_separator;

  static void cut_filename(std::string* str) {
    auto pos = str->find_last_of(_delim);

    if (pos != std::string::npos) {
      pos += 1;
      str->erase(pos, str->length() - pos);
    }
  }

  void correct_path(std::string* path) {
    if (path->front() != _delim) {
      path->insert(0, _root);
    }

    if (std::filesystem::is_directory(*path)) {
      *path += _delim;
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
