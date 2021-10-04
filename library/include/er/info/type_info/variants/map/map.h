#pragma once

#include <memory>

#include "imap.h"
#include "std_map.h"
#include "std_unordered_map.h"

namespace rr {

struct Map final : public IMap {
  Map() = delete;

  template <typename KeyT, typename ValueT>
  Map(std::map<KeyT, ValueT>* map, bool is_const)  //
      : _map(std::make_shared<StdMap<KeyT, ValueT>>(map, is_const)) {
  }

  template <typename KeyT, typename ValueT>
  Map(std::unordered_map<KeyT, ValueT>* map, bool is_const)  //
      : _map(std::make_shared<StdUnorderedMap<KeyT, ValueT>>(map, is_const)) {
  }

  Var own_var() const override {
    return _map->own_var();
  }

  TypeId key_type() const override {
    return _map->key_type();
  }

  TypeId val_type() const override {
    return _map->val_type();
  }

  void for_each(std::function<void(Var, Var)> callback) const override {
    return _map->for_each(callback);
  }

  void for_each(std::function<void(Var, Var)> callback) override {
    return _map->for_each(callback);
  }

  void clear() override {
  }

  size_t size() const override {
    return _map->size();
  }

  Expected<None> insert(Var key, Var value) override {
    return _map->insert(key, value);
  }

  Expected<None> remove(Var key) override {
    return _map->remove(key);
  }

 private:
  std::shared_ptr<IMap> _map;
};

}  // namespace rr