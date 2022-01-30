#include "executable_name.h"

#include <memory>

#if defined(__linux__)
#include <linux/limits.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

std::string executable_name() {
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
#elif defined(_WIN32)
  auto raw_path = std::make_unique<char[]>(MAX_PATH);
  auto size = GetModuleFileName(nullptr, raw_path.get(), MAX_PATH);
#endif

  return std::string(raw_path.get(), size);
}