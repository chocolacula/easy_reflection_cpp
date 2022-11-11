#pragma once

#include <array>

#include "constexpr_sort.h"
#include "expected.h"
#include "tools/format.h"

namespace er {

/// the analogue of tree based map container with fixed type of second argument
/// and the same log(N) search performance
/// it contains the same number of data: value and twe pointers
/// but in this case all of them stored in different arrays and arrays for
/// T values and names are sorted
/// which could guarantee log(N) performance with binary search
/// for both value->name and name->value access
///
/// initial compile time sorting has O(N^2) complexity
template <class T, size_t size>
class ConstexprMap {
 public:
  /// a pair like struct with deleted default constructor
  struct Entry {
    constexpr Entry() = delete;
    constexpr Entry(T value, std::string_view name) : value(value), name(name) {
    }

    T value;
    std::string_view name;
  };

  constexpr ConstexprMap(std::array<Entry, size>&& array) : _data(array), _value_ptr{}, _name_ptr{} {
    for (auto i = 0; i < _data.size(); i++) {
      _value_ptr[i] = &_data[i];
      _name_ptr[i] = &_data[i];
    }

    sort_value_ptr();
    sort_name_ptr();
  }

  Expected<std::string_view> get_name(T value) const {
    auto ptr = search_by_value(value, 0, size);

    if (ptr == nullptr) {
      return Error("Cannot find an entry");
    }

    return ptr->name;
  }

  Expected<T> get_value(std::string_view name) const {
    auto ptr = search_by_name(name, 0, size);

    if (ptr == nullptr) {
      return Error(format("Cannot find the constant '{}'", name));
    }

    return ptr->value;
  }

  [[nodiscard]] decltype(auto) begin() const {
    return _data.begin();
  }

  [[nodiscard]] decltype(auto) end() const {
    return _data.end();
  }

 private:
  std::array<Entry, size> _data;
  std::array<Entry*, size> _value_ptr;
  std::array<Entry*, size> _name_ptr;

  constexpr Entry* search_by_value(T value, size_t begin, size_t end) const {

    while (begin < end) {

      auto middleIdx = (begin + end) / 2;
      auto middleVal = _value_ptr[middleIdx]->value;

      if (value == middleVal) {
        return _value_ptr[middleIdx];
      }

      if (value < middleVal) {
        end = middleIdx;
      } else {
        begin = ++middleIdx;
      }
    }

    // nothing is found
    return nullptr;
  }

  constexpr Entry* search_by_name(std::string_view name, size_t begin, size_t end) const {

    while (begin < end) {

      auto middle_idx = (begin + end) / 2;
      auto middle_val = _name_ptr[middle_idx]->name;

      if (name == middle_val) {
        return _name_ptr[middle_idx];
      }

      if (name < middle_val) {
        end = middle_idx;
      } else {
        begin = ++middle_idx;
      }
    }

    // nothing is found
    return nullptr;
  }

  constexpr void sort_value_ptr() {
    ConstexprSort::sort(_value_ptr.data(), _value_ptr.size(),  //
                        [](auto a, auto b) { return a->value > b->value; });
  }
  constexpr void sort_name_ptr() {
    ConstexprSort::sort(_name_ptr.data(), _name_ptr.size(),  //
                        [](auto a, auto b) { return a->name > b->name; });
  }
};

}  // namespace er
