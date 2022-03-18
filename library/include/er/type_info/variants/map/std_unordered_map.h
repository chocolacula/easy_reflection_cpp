#pragma once

#include <unordered_map>

#include "er/reflection/type_name.h"
#include "imap.h"

namespace er {

template <typename KeyT, typename ValueT>
struct StdUnorderedMap final : public IMap {
  StdUnorderedMap() = delete;

  StdUnorderedMap(std::unordered_map<KeyT, ValueT>* map, bool is_const) : _map(map), _is_const(is_const) {
  }

  Expected<None> assign(Var var) override {
    auto t = TypeId::get(_map);
    if (var.type() != t) {
      return Error(format("Cannot assign type: {} to {}",     //
                          reflection::type_name(var.type()),  //
                          reflection::type_name(t)));
    }

    _map = static_cast<std::unordered_map<KeyT, ValueT>*>(const_cast<void*>(var.raw()));
    _is_const = var.is_const();
    return None();
  }

  void unsafe_assign(void* ptr) override {
    _map = static_cast<std::unordered_map<KeyT, ValueT>*>(ptr);
    _is_const = false;
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
    const auto value_type = TypeId::get<ValueT>();

    for (auto&& pair : *_map) {
      callback(Var(&pair.first), Var(&pair.second, value_type, true));
    }
  }

  void for_each(std::function<void(Var, Var)> callback) override {
    const auto value_type = TypeId::get<ValueT>();

    for (auto&& pair : *_map) {
      callback(Var(&pair.first), Var(&pair.second, value_type, _is_const));
    }
  }

  void unsafe_for_each(std::function<void(void*, void*)> callback) const override {
    for (auto&& pair : *_map) {
      callback(const_cast<KeyT*>(&pair.first), &pair.second);
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

    return k.match_move(
        [this](KeyT* ptr) -> Expected<None> {  //
          auto n = _map->erase(*ptr);

          if (n == 0) {
            return Error("The element doesn't exist");
          }

          return None();
        },
        [](Error&& err) -> Expected<None> {  //
          return err;
        });
  }

 private:
  std::unordered_map<KeyT, ValueT>* _map;
  bool _is_const;
};

}  // namespace er