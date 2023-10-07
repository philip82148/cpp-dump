/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "./escape_sequence.hpp"
#include "./type_check.hpp"
#include "./utility.hpp"

namespace cpp_dump {

extern inline size_t max_line_width;

extern inline size_t max_depth;

extern inline size_t max_iteration_count;

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
inline auto export_map(
    const T &map,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_map<T>, std::string> {
  if (map.empty()) return es::bracket("{ }", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

  bool shift_indent = is_multimap<T> || is_iterable_like<typename T::key_type>
                      || is_iterable_like<typename T::mapped_type>;
  // 中身がiterable_likeでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  size_t next_depth      = current_depth + 1;

  // iterator for values of multimap
  struct value_iterator {
    typename T::const_iterator it;
    auto operator*() { return it->second; }
    bool operator!=(const value_iterator &to) { return it != to.it; }
    value_iterator &operator++() {
      ++it;
      return *this;
    }
  };

  struct value_container {
    value_iterator _begin;
    value_iterator _end;
    auto begin() const { return _begin; }
    auto end() const { return _end; }
  };

rollback:
  std::string output     = es::bracket("{ ", current_depth);
  bool is_first          = true;
  size_t iteration_count = 0;
  for (auto it = map.begin(), end = map.end(); it != end; it = map.equal_range(it->first).second) {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    std::string key_string, value_string;
    if (shift_indent) {
      if (++iteration_count > max_iteration_count) {
        output += "\n" + new_indent + es::op("...");
        break;
      }

      if constexpr (is_multimap<T>) {
        auto [_begin, _end] = map.equal_range(it->first);
        value_container values{{_begin}, {_end}};

        key_string = "\n" + new_indent
                     + export_var(it->first, new_indent, new_indent.length(), next_depth, false)
                     + es::member(" (" + std::to_string(map.count(it->first)) + ")") + es::op(": ");
        value_string =
            export_var(values, new_indent, get_last_line_length(key_string), next_depth, false);
      } else {
        key_string = "\n" + new_indent
                     + export_var(it->first, new_indent, new_indent.length(), next_depth, false)
                     + es::op(": ");
        value_string =
            export_var(it->second, new_indent, get_last_line_length(key_string), next_depth, false);
      }

      output += key_string + value_string;
      continue;
    }

    if (++iteration_count > max_iteration_count) {
      output += es::op("...");

      if (last_line_length + get_length(output + " }") <= max_line_width) break;

      shift_indent = true;
      goto rollback;
    }

    if constexpr (is_multimap<T>) {
      auto [_begin, _end] = map.equal_range(it->first);
      value_container values{{_begin}, {_end}};

      key_string =
          export_var(it->first, indent, last_line_length + get_length(output), next_depth, true)
          + es::member(" (" + std::to_string(map.count(it->first)) + ")") + es::op(": ");
      value_string = export_var(
          values,
          indent,
          last_line_length + get_length(output) + get_length(key_string),
          next_depth,
          true
      );
    } else {
      key_string =
          export_var(it->first, indent, last_line_length + get_length(output), next_depth, true)
          + es::op(": ");
      value_string = export_var(
          it->second,
          indent,
          last_line_length + get_length(output) + get_length(key_string),
          next_depth,
          true
      );
    }

    std::string elem_string = key_string + value_string;
    if (!has_newline(elem_string)) {
      output += elem_string;

      if (last_line_length + get_length(output + " }") <= max_line_width) continue;
    }

    if (fail_on_newline) return "\n";

    shift_indent = true;
    goto rollback;
  }

  if (shift_indent) {
    output += "\n" + indent + es::bracket("}", current_depth);
  } else {
    output += es::bracket(" }", current_depth);
  }

  return output;
}

}  // namespace _detail

}  // namespace cpp_dump
