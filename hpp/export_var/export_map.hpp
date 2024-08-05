/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <string_view>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../options.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"
#include "./export_var_fwd.hpp"

namespace cpp_dump {

namespace _detail {

template <typename T>
struct _map_dummy_wrapper {
 public:
  explicit _map_dummy_wrapper(const T &map) : _map(map) {}

  auto begin() const noexcept { return _map.begin(); }
  auto end() const noexcept { return _map.end(); }
  auto size() const noexcept { return _map.size(); }

 private:
  const T &_map;
};

template <typename T>
struct _multimap_wrapper {
 public:
  explicit _multimap_wrapper(const T &map) : _begin(map, map.begin()), _end(map, map.end()) {}

  auto begin() const noexcept { return _begin; }
  auto end() const noexcept { return _end; }

 private:
  struct multimap_wrapper_iterator {
   public:
    using It = typename T::const_iterator;
    multimap_wrapper_iterator(const T &map, It it) : _map(map), _it(it) {}

    const auto &operator*() const noexcept { return *_it; }
    auto operator->() const noexcept { return _it.operator->(); }
    bool operator!=(const multimap_wrapper_iterator &to) const { return _it != to._it; }
    multimap_wrapper_iterator &operator++() {
      _it = _map.equal_range(_it->first).second;
      return *this;
    }

   private:
    const T &_map;
    It _it;
  };

  multimap_wrapper_iterator _begin;
  multimap_wrapper_iterator _end;
};

template <typename It>
struct _multimap_value_wrapper {
 public:
  _multimap_value_wrapper(It begin, It end) : _begin(begin), _end(end) {}
  _multimap_value_wrapper() = delete;

  auto begin() const noexcept { return _begin; }
  auto end() const noexcept { return _end; }

 private:
  struct multimap_value_iterator {
   public:
    explicit multimap_value_iterator(It it) : _it(it) {}
    multimap_value_iterator() = delete;

    const auto &operator*() const noexcept { return _it->second; }
    bool operator!=(const multimap_value_iterator &to) const noexcept { return _it != to._it; }
    multimap_value_iterator &operator++() {
      ++_it;
      return *this;
    }

   private:
    It _it;
  };

  multimap_value_iterator _begin;
  multimap_value_iterator _end;
};

template <typename T>
inline auto export_map(
    const T &map,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_map<T>, std::string> {
  if (map.empty()) return es::bracket("{ }", current_depth);

  if (current_depth >= options::max_depth)
    return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

  std::size_t next_depth = current_depth + 1;
  const auto &key_command = command.next_for_map_key();
  const auto &value_command = command.next_for_map_value();

  auto map_wrapper = ([&]() {
    if constexpr (is_multimap<T>) {
      return _multimap_wrapper(map);
    } else {
      // The wrapper is to avoid calling the copy constructor.
      return _map_dummy_wrapper(map);
    }
  })();
  auto skipped_map = command.create_skip_container(map_wrapper);

  bool shift_indent;
  if (options::cont_indent_style == cont_indent_style_t::always) {
    shift_indent = true;
  } else if (options::cont_indent_style == cont_indent_style_t::when_nested) {
    shift_indent = is_multimap<T> || is_iterable_like<typename T::key_type>
                   || is_iterable_like<typename T::mapped_type>;
  } else if (options::cont_indent_style == cont_indent_style_t::when_non_tuples_nested) {
    shift_indent =
        is_multimap<T>
        || (is_iterable_like<typename T::key_type> && !is_tuple<typename T::key_type>)
        || (is_iterable_like<typename T::mapped_type> && !is_tuple<typename T::mapped_type>);
  } else {
    shift_indent = false;
  }

  if (!shift_indent) {
    std::string output = es::bracket("{ ", current_depth);
    bool is_first_elem = true;

    for (const auto &[is_ellipsis, it, _] : skipped_map) {
      const auto &[key, value] = *it;

      if (is_first_elem) {
        is_first_elem = false;
      } else {
        output += es::op(", ");
      }

      if (is_ellipsis) {
        output += es::op("...");

        if (last_line_length + get_length(output) + std::string_view(" }").size()
            > options::max_line_width) {
          shift_indent = true;
          break;
        }

        continue;
      }

      std::string elem_str;
      if constexpr (is_multimap<T>) {
        auto [_begin, _end] = map.equal_range(key);
        _multimap_value_wrapper values(_begin, _end);

        // Treat the multiplicity as a member to distinguish it from the keys & values.
        // Also, multiplicities are similar to members since they are on the left side of values.
        std::string key_str =
            export_var(
                key, indent, last_line_length + get_length(output), next_depth, true, key_command
            )
            + es::member(" (" + std::to_string(map.count(key)) + ")") + es::op(": ");
        std::string value_str = export_var(
            values,
            indent,
            last_line_length + get_length(output) + get_length(key_str),
            next_depth,
            true,
            value_command
        );
        elem_str = key_str + value_str;
      } else {
        std::string key_str =
            export_var(
                key, indent, last_line_length + get_length(output), next_depth, true, key_command
            )
            + es::op(": ");
        std::string value_str = export_var(
            value,
            indent,
            last_line_length + get_length(output) + get_length(key_str),
            next_depth,
            true,
            value_command
        );
        elem_str = key_str + value_str;
      }

      if (has_newline(elem_str)) {
        shift_indent = true;
        break;
      }

      output += elem_str;
      if (last_line_length + get_length(output) + std::string_view(" }").size()
          > options::max_line_width) {
        shift_indent = true;
        break;
      }
    }

    if (!shift_indent) {
      output += es::bracket(" }", current_depth);

      return output;
    }
  }

  if (fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";

  std::string output = es::bracket("{", current_depth);
  bool is_first_elem = true;

  for (const auto &[is_ellipsis, it, _] : skipped_map) {
    const auto &[key, value] = *it;

    if (is_first_elem) {
      is_first_elem = false;
    } else {
      output += es::op(",");
    }

    if (is_ellipsis) {
      output += "\n" + new_indent + es::op("...");
      continue;
    }

    if constexpr (is_multimap<T>) {
      auto [_begin, _end] = map.equal_range(key);
      _multimap_value_wrapper values(_begin, _end);

      // Treat the multiplicity as a member to distinguish it from the keys & values.
      // Also, multiplicities are similar to members since they are on the left side of values.
      std::string key_str =
          "\n" + new_indent
          + export_var(key, new_indent, new_indent.length(), next_depth, false, key_command)
          + es::member(" (" + std::to_string(map.count(key)) + ")") + es::op(": ");
      std::string value_str = export_var(
          values, new_indent, get_last_line_length(key_str), next_depth, false, value_command
      );

      output += key_str + value_str;
    } else {
      std::string key_str =
          "\n" + new_indent
          + export_var(key, new_indent, new_indent.length(), next_depth, false, key_command)
          + es::op(": ");
      std::string value_str = export_var(
          value, new_indent, get_last_line_length(key_str), next_depth, false, value_command
      );

      output += key_str + value_str;
    }
  }
  output += "\n" + indent + es::bracket("}", current_depth);

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
