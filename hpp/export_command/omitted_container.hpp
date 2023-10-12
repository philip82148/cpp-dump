/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <utility>

#include "../iterable.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename T>
struct omitted_container {
 public:
  omitted_container(
      const T &container,
      const std::shared_ptr<std::function<
          std::pair<bool, std::optional<std::size_t>>(std::size_t, std::function<std::size_t()>)>>
          &it_info_ptr
  )
      : original(container),
        it_info_ptr(it_info_ptr),
        _begin(*this, iterable_begin(container)),
        _end(*this, iterable_end(container)) {}
  omitted_container() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  template <typename It>
  struct omitted_iterator {
   public:
    omitted_iterator(const omitted_container<T> &parent, const It &it)
        : parent(parent), it(it), index(0), done(false) {}
    omitted_iterator() = delete;

    std::pair<bool, It> operator*() const {
      auto [is_ellipsis, next_advance] = it_info();
      return {is_ellipsis, it};
    }
    template <typename U>
    bool operator!=(const U &to) const {
      return !done && it != to.it;
    }
    omitted_iterator &operator++() {
      auto [is_ellipsis, next_advance] = it_info();

      if (next_advance) {
        iterator_advance(it, next_advance.value());
        index += next_advance.value();
      } else {
        done = true;
      }

      return *this;
    }

   private:
    const omitted_container<T> &parent;
    It it;
    std::size_t index;
    bool done;
    mutable std::optional<std::size_t> size;

    std::pair<bool, std::optional<std::size_t>> it_info() const {
      return parent.it_info_ptr->operator()(index, [this] { return original_size(); });
    }

    std::size_t original_size() const {
      if (!size) size = iterable_size(parent.original);
      return size.value();
    }
  };

  const T &original;
  const std::shared_ptr<std::function<
      std::pair<bool, std::optional<std::size_t>>(std::size_t, std::function<std::size_t()>)>>
      it_info_ptr;
  const omitted_iterator<decltype(iterable_begin(original))> _begin;
  const omitted_iterator<decltype(iterable_end(original))> _end;
};

namespace it_info_aux {

inline std::pair<bool, std::optional<std::size_t>> keep_front(
    std::size_t index, std::function<std::size_t()>, std::size_t iteration_count
) {
  bool is_ellipsis                        = index >= iteration_count;
  std::optional<std::size_t> next_advance = 1;
  if (is_ellipsis) next_advance = std::nullopt;

  return {is_ellipsis, next_advance};
}

inline std::pair<bool, std::optional<std::size_t>> keep_back(
    std::size_t index, std::function<std::size_t()> get_size, std::size_t iteration_count
) {
  std::size_t size  = get_size();
  std::size_t first = size >= iteration_count ? size - iteration_count : 0;

  bool is_ellipsis                        = index < first;
  std::optional<std::size_t> next_advance = 1;
  if (is_ellipsis) next_advance = first - index;

  return {is_ellipsis, next_advance};
}

inline std::pair<bool, std::optional<std::size_t>> keep_both_ends(
    std::size_t index, std::function<std::size_t()> get_size, std::size_t iteration_count
) {
  std::size_t size              = get_size();
  std::size_t first_half_last   = (iteration_count + 1) / 2;
  std::size_t rest_count        = iteration_count - first_half_last;
  std::size_t latter_half_first = size >= rest_count ? size - rest_count : 0;

  bool is_ellipsis                        = index >= first_half_last && index < latter_half_first;
  std::optional<std::size_t> next_advance = 1;
  if (is_ellipsis) next_advance = latter_half_first - index;

  return {is_ellipsis, next_advance};
}

inline std::pair<bool, std::optional<std::size_t>> keep_middle(
    std::size_t index, std::function<std::size_t()> get_size, std::size_t iteration_count
) {
  std::size_t size  = get_size();
  std::size_t first = size >= iteration_count ? (size - iteration_count) / 2 : 0;
  std::size_t last  = first + iteration_count;

  bool is_ellipsis                        = index < first || index >= last;
  std::optional<std::size_t> next_advance = 1;
  if (is_ellipsis) {
    if (index < first) {
      next_advance = first - index;
    } else {
      next_advance = std::nullopt;
    }
  }

  return {is_ellipsis, next_advance};
}

}  // namespace it_info_aux

}  // namespace _detail

}  // namespace cpp_dump
