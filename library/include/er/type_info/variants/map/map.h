#pragma once

#include "er/tools/sizeof.h"
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
    return impl()->assign(var);
  }

  void unsafe_assign(void* ptr) {
    impl()->unsafe_assign(ptr);
  }

  Var own_var() const {
    return impl()->own_var();
  }

  TypeId key_type() const {
    return impl()->key_type();
  }

  TypeId val_type() const {
    return impl()->val_type();
  }

  void for_each(std::function<void(Var, Var)> callback) const {
    return impl()->for_each(callback);
  }

  void for_each(std::function<void(Var, Var)> callback) {
    return impl()->for_each(callback);
  }

  void unsafe_for_each(std::function<void(void*, void*)> callback) const {
    return impl()->unsafe_for_each(callback);
  }

  void clear() {
    impl()->clear();
  }

  size_t size() const {
    return impl()->size();
  }

  Expected<None> insert(Var key, Var value) {
    return impl()->insert(key, value);
  }

  Expected<None> remove(Var key) {
    return impl()->remove(key);
  }

 private:
  char _mem[Sizeof<StdMap<int, int>, StdUnorderedMap<int, int>>::max];

  inline const IMap* impl() const {
    return reinterpret_cast<const IMap*>(&_mem[0]);
  }

  inline IMap* impl() {
    return reinterpret_cast<IMap*>(&_mem[0]);
  }
};

}  // namespace er
