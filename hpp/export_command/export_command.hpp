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
#include "./skip_container.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

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

template <typename T>
struct value_with_command;

struct export_command {
  friend export_command _map_key(const export_command &command) {
    export_command new_command;
    new_command.map_key_skip_size_func = command.skip_size_func;
    new_command.map_key_child          = command.child;
    return new_command;
  }

  friend export_command _map_value(const export_command &command) {
    export_command new_command;
    new_command.map_value_skip_size_func = command.skip_size_func;
    new_command.map_value_child          = command.child;
    return new_command;
  }

  friend export_command _map_key_and_value(const export_command &key, const export_command &value) {
    export_command new_command;
    new_command.map_key_skip_size_func   = key.skip_size_func;
    new_command.map_key_child            = key.child;
    new_command.map_value_skip_size_func = value.skip_size_func;
    new_command.map_value_child          = value.child;
    return new_command;
  }

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
  export_command(
      const std::shared_ptr<std::function<
          std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
          &skip_size_func,
      const std::shared_ptr<export_command> &child
  )
      : skip_size_func(skip_size_func), child(child) {}
  export_command(const export_command &)            = default;
  export_command &operator=(const export_command &) = default;
  export_command() : skip_size_func(_default_skip_size_func) {}

  template <typename T>
  skip_container<T> get_skip_container(const T &container) const {
    return skip_container<T>(container, *skip_size_func);
  }

  export_command next() const {
    if (child) return *child;

    return export_command();
  }

  export_command next_for_map_key() const {
    if (map_key_child) return *map_key_child;

    return next();
  }

  export_command next_for_map_value() const {
    if (map_value_child) return *map_value_child;

    return next();
  }

  template <typename T>
  value_with_command<T> operator<<(const T &value) {
    return value_with_command<T>(value, *this);
  }

  export_command &operator<<(const export_command &command) {
    if (child) {
      *child << command;
    } else {
      child.reset(new export_command(command.skip_size_func, command.child));

      if (command.map_key_skip_size_func)
        map_key_child.reset(
            new export_command(command.map_key_skip_size_func, command.map_key_child)
        );

      if (command.map_value_skip_size_func)
        map_value_child.reset(
            new export_command(command.map_value_skip_size_func, command.map_value_child)
        );
    }

    return *this;
  }

 private:
  std::shared_ptr<
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
      skip_size_func;
  std::shared_ptr<export_command> child;
  std::shared_ptr<
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
      map_key_skip_size_func;
  std::shared_ptr<export_command> map_key_child;
  std::shared_ptr<
      std::function<std::optional<std::size_t>(std::size_t, const std::function<std::size_t()> &)>>
      map_value_skip_size_func;
  std::shared_ptr<export_command> map_value_child;
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

auto map_key(const _detail::export_command &command) { return _map_key(command); }

auto map_value(const _detail::export_command &command) { return _map_value(command); }

auto map_key_and_value(const _detail::export_command &key, const _detail::export_command &value) {
  return _map_key_and_value(key, value);
}

}  // namespace cpp_dump
