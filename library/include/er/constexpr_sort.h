#include <sys/types.h>

#include <algorithm>
#include <cstddef>

struct ConstexprSort {
  // Has optimized complexity
  template <typename T>
  static constexpr void sort(T* arr, size_t size) {
    sort(arr, size, [](T a, T b) { return a > b; });
  }

  // Has optimized complexity
  template <typename T, typename LambdaT>
  static constexpr void sort(T* arr, size_t size, LambdaT greater) {
    if (size > 32) {
      quick_sort(arr, size, greater);
    } else {
      insertion_sort(arr, size, greater);
    }
  }

  // Has complexity O(N^2)
  template <typename T>
  static constexpr void insertion_sort(T* arr, size_t size) {
    insertion_sort(arr, size, [](T a, T b) { return a > b; });
  }

  // Has complexity O(N^2)
  template <typename T, typename LambdaT>
  static constexpr void insertion_sort(T* arr, size_t size, LambdaT comp) {
    for (auto i = 1; i < size; i++) {
      auto key = arr[i];

      auto j = i;
      while (j > 0 && comp(arr[j - 1], key)) {
        arr[j] = arr[j - 1];
        j--;
      }
      arr[j] = key;
    }
  }

  // Has complexity O(log(N))
  template <typename T>
  static constexpr void quick_sort(T* arr, size_t size) {
    quick_sort(arr, size, [](T a, T b) { return a > b; });
  }

  // Has complexity O(log(N))
  template <typename T, typename LambdaT>
  static constexpr void quick_sort(T* arr, size_t size, LambdaT greater) {
    quick_sort_req(arr, 0, size - 1, greater);
  }

 private:
  template <typename T, typename LambdaT>
  static constexpr void quick_sort_req(T* arr, ssize_t begin, ssize_t end, LambdaT greater) {
    if (begin >= end) {
      return;
    }
    auto pivot = hoare(arr, begin, end, greater);

    quick_sort_req(arr, begin, pivot, greater);
    quick_sort_req(arr, pivot + 1, end, greater);
  }

  template <typename T, typename LambdaT>
  static constexpr size_t hoare(T* arr, ssize_t begin, ssize_t end, LambdaT greater) {
    auto pivot = arr[(begin + end) / 2];

    auto i = begin - 1;
    auto j = end + 1;

    while (true) {
      do {
        i++;
      } while (greater(pivot, arr[i]));

      do {
        j--;
      } while (greater(arr[j], pivot));

      if (i >= j) {
        return j;
      }

      std::swap(arr[i], arr[j]);
    }
  }
};
