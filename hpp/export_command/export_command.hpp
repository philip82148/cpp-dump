/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "../type_check.hpp"
#include "./omitted_container.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename T>
struct value_with_command;

const std::shared_ptr<
    std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
    _default_skip_size_func(
        std::make_shared<std::function<
            std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>(
            [](std::size_t index, std::function<std::size_t()> get_size) {
              return skip_size_func::keep_front(index, get_size, max_iteration_count);
            }
        )
    );

struct export_command {
 public:
  export_command(
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>
          &&skip_size_func
  )
      : skip_size_func(
          std::make_shared<std::function<
              std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>(
              std::move(skip_size_func)
          )
      ) {}
  export_command(const export_command &command)
      : skip_size_func(command.skip_size_func), child(command.child) {}
  export_command() : skip_size_func(_default_skip_size_func) {}

  template <typename T>
  omitted_container<T> get_omitted_container(const T &container) const {
    return omitted_container<T>(container, *skip_size_func);
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
      child = std::make_shared<export_command>(command);
    }
    return *this;
  }

 private:
  const std::shared_ptr<
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
      skip_size_func;
  std::shared_ptr<export_command> child;
};

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

inline auto keep_front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_front(index, get_size, iteration_count);
  });
}

inline auto keep_back(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_back(index, get_size, iteration_count);
  });
}

inline auto keep_both_ends(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_both_ends(index, get_size, iteration_count);
  });
}

inline auto keep_middle(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index,
                                     const std::function<std::size_t()> &get_size) {
    return _detail::skip_size_func::keep_middle(index, get_size, iteration_count);
  });
}

}  // namespace cpp_dump
