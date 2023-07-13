#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "er/error/error.h"
#include "er/type_id.h"
#include "method_desc.h"

namespace er {

class MethodInfo {
 public:
  MethodInfo(const void* base, const MethodDesc* data) : _base(const_cast<void*>(base)), _data(data) {
  }

  MethodInfo(void* base, const MethodDesc* data) : _base(base), _data(data) {
  }

  MethodInfo(const MethodInfo& other) {
    if (this == &other) {
      return;
    }
    _base = other._base;
    _data = other._data;
  }

  MethodInfo& operator=(const MethodInfo& other) {
    if (this == &other) {
      return *this;
    }
    _base = other._base;
    _data = other._data;
    return *this;
  }

  template <typename... Args>
  Expected<None> invoke(const Args&... args) const {
    std::vector<Var> v_args;
    fold_args(&v_args, args...);

    return _data->invoke(nullptr, _base, v_args);
  }

  template <typename RetT, typename... Args>
  Expected<RetT> invoke(const Args&... args) const {
    std::vector<Var> v_args;
    fold_args(&v_args, args...);

    RetT ret;
    return _data->invoke(&ret, _base, v_args)
        .match_move(                                            //
            [](Error&& err) -> Expected<RetT> { return err; },  //
            [&](auto&& _) -> Expected<RetT> { return ret; });
  }

  bool is_const() const {
    return _data->is_const();
  }

  bool is_static() const {
    return _data->is_static();
  }

  bool is_public() const {
    return _data->is_public();
  }

  bool is_protected() const {
    return _data->is_protected();
  }

  bool is_private() const {
    return _data->is_private();
  }

 private:
  void* _base;
  const MethodDesc* _data;

  template <typename ArgT, typename... Args>
  void fold_args(std::vector<Var>* v_args, const ArgT& arg, const Args&... other) const {
    v_args->push_back(Var(&arg));

    fold_args(v_args, other...);
  }

  template <typename ArgT>
  void fold_args(std::vector<Var>* v_args, const ArgT& arg) const {

    v_args->push_back(Var(&arg));
  }
};

}  // namespace er
