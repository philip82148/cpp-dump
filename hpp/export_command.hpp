/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "./iterable.hpp"
#include "./type_check.hpp"

namespace cpp_dump {

extern inline size_t max_iteration_count;

namespace _detail {

template <typename T>
struct omitted_container {
 public:
  omitted_container(
      const T &container,
      const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>> &is_valid
  )
      : original(container),
        is_valid(is_valid),
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
        : parent(parent), it(it), index(0) {}
    omitted_iterator() = delete;

    auto operator*() const { return std::make_pair(is_ellipsis(), *it); }
    template <typename U>
    bool operator!=(const U &to) const {
      return it != to.it;
    }
    omitted_iterator &operator++() {
      if (is_ellipsis()) {
        while (it != parent._end.it && is_ellipsis()) {
          ++it;
          ++index;
        }
      } else {
        ++it;
        ++index;
      }

      return *this;
    }

   private:
    const omitted_container<T> &parent;
    It it;
    size_t index;

    bool is_ellipsis() const {
      return !parent.is_valid->operator()(index, [this] { return original_size(); });
    }

    size_t original_size() const {
      static const size_t size = iterable_size(parent.original);
      return size;
    }
  };

  const T &original;
  const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>> is_valid;
  const omitted_iterator<decltype(iterable_begin(original))> _begin;
  const omitted_iterator<decltype(iterable_end(original))> _end;
};

template <typename T>
struct value_with_command;

struct export_command {
 public:
  export_command(std::function<bool(size_t, std::function<size_t()>)> &&is_valid)
      : is_valid(std::make_shared<std::function<bool(size_t, std::function<size_t()>)>>(
          std::move(is_valid)
      )) {}
  export_command(const export_command &command)
      : is_valid(command.is_valid), child(command.child) {}
  export_command() : is_valid(default_is_valid) {}

  template <typename T>
  omitted_container<T> get_omitted_container(const T &container) const {
    return omitted_container<T>(container, is_valid);
  }

  export_command next() const {
    if (child) return *child;

    return export_command();
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) {
    return value_with_command<T>(value, *this);
  }

  export_command &operator<<(export_command &&command) {
    if (child) {
      *child << std::move(command);
    } else {
      child.reset(&command);
    }
    return *this;
  }

 private:
  const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>> is_valid;
  std::shared_ptr<export_command> child;
  static const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>>
      default_is_valid;
};

const std::shared_ptr<std::function<bool(size_t, std::function<size_t()>)>>
    export_command::default_is_valid(
        std::make_shared<std::function<bool(size_t, std::function<size_t()>)>>(
            [](size_t index, std::function<size_t()>) { return index < max_iteration_count; }
        )
    );

template <typename T>
struct value_with_command {
 public:
  const T &value;
  const export_command command;

  value_with_command(const T &value, const export_command &command)
      : value(value), command(command) {}
  value_with_command() = delete;
};

template <typename>
inline constexpr bool _is_value_with_command = false;
template <typename T>
inline constexpr bool _is_value_with_command<value_with_command<T>> = true;

template <typename T>
inline constexpr bool is_value_with_command = _is_value_with_command<_remove_cref<T>>;

}  // namespace _detail

inline auto omit_front(size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](size_t index, std::function<size_t()> get_size) -> bool {
    size_t first = get_size() - iteration_count;
    return index >= first;
  });
}

inline auto omit_middle(size_t half_iteration_count = max_iteration_count / 2) {
  return _detail::export_command([=](size_t index, std::function<size_t()> get_size) -> bool {
    size_t latter_half_first = get_size() - half_iteration_count;
    return index < half_iteration_count || index >= latter_half_first;
  });
}

inline auto omit_back(size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](size_t index, std::function<size_t()>) {
    return index < iteration_count;
  });
}

}  // namespace cpp_dump
