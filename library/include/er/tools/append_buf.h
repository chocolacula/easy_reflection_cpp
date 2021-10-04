#pragma once

#include <streambuf>

namespace rr {

class AppendBuf : public std::streambuf {
 public:
  explicit AppendBuf(std::string* str) : _buf(str) {
  }

  int_type overflow(int_type c) override {
    if (c != EOF) {
      _buf->push_back(c);
    }
    return c;
  }

  std::streamsize xsputn(const char* s, std::streamsize n) override {
    _buf->append(s, s + n);
    return n;
  }

 private:
  std::string* _buf;
};

}  // namespace rr
