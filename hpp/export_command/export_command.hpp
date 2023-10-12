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

#include "../type_check.hpp"
#include "./omitted_container.hpp"

namespace cpp_dump {

extern inline std::size_t max_iteration_count;

namespace _detail {

template <typename T>
struct value_with_command;

struct export_command {
 public:
  export_command(std::function<bool(std::size_t, std::function<std::size_t()>)> &&is_valid)
      : is_valid(std::make_shared<std::function<bool(std::size_t, std::function<std::size_t()>)>>(
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
      child = std::make_shared<export_command>(command);
    }
    return *this;
  }

 private:
  const std::shared_ptr<std::function<bool(std::size_t, std::function<std::size_t()>)>> is_valid;
  std::shared_ptr<export_command> child;
  static const std::shared_ptr<std::function<bool(std::size_t, std::function<std::size_t()>)>>
      default_is_valid;
};

const std::shared_ptr<std::function<bool(std::size_t, std::function<std::size_t()>)>>
    export_command::default_is_valid(
        std::make_shared<std::function<bool(std::size_t, std::function<std::size_t()>)>>(
            [](std::size_t index, std::function<std::size_t()>) {
              return index < max_iteration_count;
            }
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

inline auto keep_front(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command([=](std::size_t index, std::function<std::size_t()>) {
    return index < iteration_count;
  });
}

inline auto keep_back(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, std::function<std::size_t()> get_size) -> bool {
        std::size_t size  = get_size();
        std::size_t first = size >= iteration_count ? size - iteration_count : 0;

        return index >= first;
      }
  );
}

inline auto keep_both_ends(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, std::function<std::size_t()> get_size) -> bool {
        std::size_t size              = get_size();
        std::size_t first_half_last   = (iteration_count + 1) / 2;
        std::size_t rest_count        = iteration_count - first_half_last;
        std::size_t latter_half_first = size >= rest_count ? size - rest_count : 0;

        return index < first_half_last || index >= latter_half_first;
      }
  );
}

inline auto keep_middle(std::size_t iteration_count = max_iteration_count) {
  return _detail::export_command(
      [=](std::size_t index, std::function<std::size_t()> get_size) -> bool {
        std::size_t size  = get_size();
        std::size_t first = size >= iteration_count ? (size - iteration_count) / 2 : 0;
        std::size_t last  = first + iteration_count;

        return index >= first && index < last;
      }
  );
}

}  // namespace cpp_dump