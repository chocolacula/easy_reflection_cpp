#pragma once

#include <map>

#include "imap.h"

namespace rr {

template <typename KeyT, typename ValueT>
struct StdMap final : public IMap {
  StdMap() = delete;

  StdMap(std::map<KeyT, ValueT>* map, bool is_const) : _map(map), _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_map, TypeId::get(_map), _is_const);
  }

  TypeId key_type() const override {
    return TypeId::get<KeyT>();
  }

  TypeId val_type() const override {
    return TypeId::get<ValueT>();
  }

  void for_each(std::function<void(Var, Var)> callback) const override {
    auto value_type = TypeId::get<ValueT>();

    for (auto&& pair : *_map) {
      callback(Var(&pair.first), Var(&pair.second, value_type, true));
    }
  }

  void for_each(std::function<void(Var, Var)> callback) override {
    auto value_type = TypeId::get<ValueT>();

    for (auto&& pair : *_map) {
      callback(Var(&pair.first), Var(&pair.second, value_type, _is_const));
    }
  }

  void clear() override {
    _map->clear();
  }

  size_t size() const override {
    return _map->size();
  }

  Expected<None> insert(Var key, Var value) override {
    auto k = key.rt_cast<KeyT>();
    if (k.is_error()) {
      return k.template get<Error>();
    }

    auto v = value.rt_cast<ValueT>();
    if (v.is_error()) {
      return v.template get<Error>();
    }

    auto r = _map->insert(std::make_pair(*k.unwrap(), *v.unwrap()));

    if (r.second != true) {
      return Error("The pair of key and value does already exist");
    }

    return None();
  }

  Expected<None> remove(Var key) override {

    auto k = key.rt_cast<KeyT>();

    return k.match(
        [this](KeyT* ptr) -> Expected<None> {  //
          auto n = _map->erase(*ptr);

          if (n == 0) {
            return Error("The element doesn't exist");
          }

          return None();
        },
        [](Error err) -> Expected<None> {  //
          return err;
        });
  }

 private:
  std::map<KeyT, ValueT>* _map;
  bool _is_const;
};

}  // namespace rr