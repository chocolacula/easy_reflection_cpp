#pragma once

#include "imap.h"
#include "std_map.h"
#include "std_unordered_map.h"

namespace er {

struct Map final {
  Map() = delete;

  template <typename KeyT, typename ValueT>
  Map(std::map<KeyT, ValueT>* map, bool is_const) {
    new (_mem) StdMap<KeyT, ValueT>(map, is_const);
  }

  template <typename KeyT, typename ValueT>
  Map(std::unordered_map<KeyT, ValueT>* map, bool is_const) {
    new (_mem) StdUnorderedMap<KeyT, ValueT>(map, is_const);
  }

  Expected<None> assign(Var var) {
    return reinterpret_cast<IMap*>(&_mem[0])->assign(var);
  }

  void unsafe_assign(void* ptr) {
    reinterpret_cast<IMap*>(&_mem[0])->unsafe_assign(ptr);
  }

  Var own_var() const {
    return reinterpret_cast<const IMap*>(&_mem[0])->own_var();
  }

  TypeId key_type() const {
    return reinterpret_cast<const IMap*>(&_mem[0])->key_type();
  }

  TypeId val_type() const {
    return reinterpret_cast<const IMap*>(&_mem[0])->val_type();
  }

  void for_each(std::function<void(Var, Var)> callback) const {
    return reinterpret_cast<const IMap*>(&_mem[0])->for_each(callback);
  }

  void for_each(std::function<void(Var, Var)> callback) {
    return reinterpret_cast<IMap*>(&_mem[0])->for_each(callback);
  }

  void unsafe_for_each(std::function<void(void*, void*)> callback) const {
    return reinterpret_cast<const IMap*>(&_mem[0])->unsafe_for_each(callback);
  }

  void clear() {
    reinterpret_cast<IMap*>(&_mem[0])->clear();
  }

  size_t size() const {
    return reinterpret_cast<const IMap*>(&_mem[0])->size();
  }

  Expected<None> insert(Var key, Var value) {
    return reinterpret_cast<IMap*>(&_mem[0])->insert(key, value);
  }

  Expected<None> remove(Var key) {
    return reinterpret_cast<IMap*>(&_mem[0])->remove(key);
  }

 private:
  // a little hack to reduce dynamic memory allocation
  // this approach is little faster then use shared_ptr but still faster
  //
  // it's just a memory bunch for a pointer and is_const flag
  // all kinds of map has the same sizeof()
  char _mem[sizeof(StdMap<int, int>)];
};

}  // namespace er