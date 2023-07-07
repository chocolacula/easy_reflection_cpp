#pragma once

#include <type_traits>

#include "er/tools/traits.h"
#include "er/type_id.h"
#include "er/variable/var.h"
#include "method_attributes.h"

namespace er {

struct MethodDesc {
 public:
  MethodDesc(Expected<None> (*fn)(void* res, void* obj, const std::vector<Var>& args), MethodAttributes acc)
      : _fn(fn), _atr(acc) {
  }

  Expected<None> invoke(void* res, void* obj, const std::vector<Var>& args) const {
    return _fn(res, obj, args);
  }

  bool is_const() const {
    return (_atr & MethodAttributes::kStatic) != MethodAttributes::kNone;
  }

  bool is_static() const {
    return (_atr & MethodAttributes::kStatic) != MethodAttributes::kNone;
  }

  bool is_public() const {
    return (_atr & MethodAttributes::kPublic) != MethodAttributes::kNone;
  }

  bool is_protected() const {
    return (_atr & MethodAttributes::kProtected) != MethodAttributes::kNone;
  }

  bool is_private() const {
    return (_atr & MethodAttributes::kPrivate) != MethodAttributes::kNone;
  }

  MethodAttributes attributes() const {
    return _atr;
  }

 private:
  Expected<None> (*_fn)(void* res, void* obj, const std::vector<Var>& args);
  MethodAttributes _atr;
};

}  // namespace er
