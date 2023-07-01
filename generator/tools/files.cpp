#include "files.h"

#include <filesystem>
#include <memory>
#include <vector>

#if defined(__linux__)
#include <linux/limits.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>

#include <algorithm>
#endif

#include "../config.h"

Files::Files() {
  _root = executable_name();
  cut_filename(&_root);
}

const std::string& Files::root() {
  return _root;
}

void Files::correct_config(Config* config) {
  correct_path(&(config->compdb_dir));
  correct_path(&(config->templates.header));
  correct_path(&(config->templates.for_enum));
  correct_path(&(config->templates.object));
  correct_path(&(config->output_dir));

  complete_files(&(config->input));
}

#if defined(_WIN32)
std::string Files::to_utf8(const wchar_t* str, size_t size) {
  int size_utf8 = WideCharToMultiByte(CP_UTF8,                 //
                                      WC_ERR_INVALID_CHARS,    //
                                      str,                     //
                                      static_cast<int>(size),  //
                                      nullptr, 0, nullptr, nullptr);

  std::string str_utf8(size_utf8, '\0');
  WideCharToMultiByte(CP_UTF8,                 //
                      WC_ERR_INVALID_CHARS,    //
                      str,                     //
                      static_cast<int>(size),  //
                      str_utf8.data(),         //
                      size_utf8,               //
                      nullptr, nullptr);

  return str_utf8;
}

std::wstring Files::from_utf8(const char* str, size_t size) {
  int size_w = MultiByteToWideChar(CP_UTF8,                 //
                                   MB_ERR_INVALID_CHARS,    //
                                   str,                     //
                                   static_cast<int>(size),  //
                                   nullptr, 0);

  std::wstring str_w(size_w, '\0');
  MultiByteToWideChar(CP_UTF8,                 //
                      MB_ERR_INVALID_CHARS,    //
                      str,                     //
                      static_cast<int>(size),  //
                      str_w.data(),            //
                      size_w);

  return str_w;
}
#endif

inline std::string Files::executable_name() {
#if defined(__linux__)
  auto raw_path = std::make_unique<char[]>(PATH_MAX);

  auto size = readlink("/proc/self/exe", raw_path.get(), PATH_MAX);
  if (size == -1) {
    size = 0;
  }

  return std::string(raw_path.get(), size);
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

  return std::string(raw_path.get(), size);
#elif defined(_WIN32)
  std::wstring str_raw(MAX_PATH, '\0');
  // int size_raw = GetModuleFileNameW(nullptr, &str_raw[0], MAX_PATH);

  auto raw_path = std::make_unique<wchar_t[]>(MAX_PATH);
  auto size = GetModuleFileNameW(nullptr, raw_path.get(), MAX_PATH);

  auto utf8_path = to_utf8(raw_path.get(), size);

  // use '/' even on Windows
  std::replace(utf8_path.begin(), utf8_path.end(), '\\', '/');

  return utf8_path;
#endif
}

void Files::correct_path(std::string* path) {
  if (!is_absolute(*path)) {
    path->insert(0, _root);
  }

  if (path->back() != kDelim) {
#if defined(_WIN32)
    std::filesystem::path fs_p(from_utf8(path->data(), path->size()));
#else
    std::filesystem::path fs_p(*path);
#endif
    if (std::filesystem::is_directory(fs_p)) {
      *path += kDelim;
    }
  }
}

void Files::complete_files(std::vector<std::string>* paths) {
  auto old = *paths;
  paths->clear();

  for (auto path : old) {
    correct_path(&path);

#if defined(_WIN32)
    std::filesystem::path fs_path(from_utf8(path.data(), path.size()));
#else
    std::filesystem::path fs_path(path);
#endif

    if (std::filesystem::is_directory(fs_path)) {
      for (auto&& file_path : std::filesystem::recursive_directory_iterator(fs_path)) {
#if defined(_WIN32)
        auto w_str = file_path.path().wstring();
        paths->push_back(to_utf8(w_str.data(), w_str.size()));
#else
        paths->push_back(file_path.path().string());
#endif
      }
    } else {
      paths->push_back(path);
    }
  }
}

void Files::cut_filename(std::string* str) {
  auto pos = str->find_last_of(kDelim);

  if (pos != std::string::npos) {
    pos += 1;
    str->erase(pos, str->length() - pos);
  }
}

inline bool Files::is_absolute(const std::string& path) {
#if defined(_WIN32)
  return path[1] == ':';  // match 'C:\', 'D:\', etc
#else
  return path.front() == kDelim;
#endif
}
