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

namespace _detail {

template <typename T>
std::string export_var(const T &, const std::string &, size_t, size_t, bool);

template <typename T>
struct _map_wrapper {
 public:
  _map_wrapper(const T &map) : _begin(map, map.begin()), _end(map, map.end()) {}
  _map_wrapper() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  struct map_wrapper_iterator {
   public:
    using It = typename T::const_iterator;
    map_wrapper_iterator(const T &map, It it) : map(map), it(it) {}
    map_wrapper_iterator() = delete;

    auto operator*() const { return *it; }
    auto operator->() { return it.operator->(); }
    bool operator!=(const map_wrapper_iterator &to) const { return it != to.it; }
    map_wrapper_iterator &operator++() {
      it = map.equal_range(it->first).second;
      return *this;
    }

   private:
    const T &map;
    It it;
  };

  map_wrapper_iterator _begin;
  map_wrapper_iterator _end;
};

template <typename It>
struct _multimap_value_wrapper {
 public:
  _multimap_value_wrapper(It begin, It end) : _begin(begin), _end(end) {}
  _multimap_value_wrapper() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  struct multimap_value_iterator {
   public:
    multimap_value_iterator(It it) : it(it) {}
    multimap_value_iterator() = delete;

    auto operator*() const { return it->second; }
    bool operator!=(const multimap_value_iterator &to) const { return it != to.it; }
    multimap_value_iterator &operator++() {
      ++it;
      return *this;
    }

   private:
    It it;
  };

  multimap_value_iterator _begin;
  multimap_value_iterator _end;
};

template <typename T>
inline auto export_map(
    const omitted_container<T> &omitted_map,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_map<T>, std::string> {
  if (omitted_map.empty()) return es::bracket("{ }", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

  bool shift_indent = is_multimap<T> || is_iterable_like<typename T::key_type>
                      || is_iterable_like<typename T::mapped_type>;
  // 中身がiterable_likeでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  size_t next_depth      = current_depth + 1;

  const T &map = omitted_map.original;
  omitted_container<_map_wrapper<T>> omitted_map_wrapper(
      _map_wrapper<T>(map), omitted_map.is_valid
  );

rollback:
  std::string output = es::bracket("{ ", current_depth);
  bool is_first      = true;

  auto it  = omitted_map_wrapper.begin();
  auto end = omitted_map_wrapper.end();
  for (; it != end; ++it) {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    std::string key_string, value_string;
    if (shift_indent) {
      if (it.is_ellipsis()) {
        output += "\n" + new_indent + es::op("...");
        continue;
      }

      if constexpr (is_multimap<T>) {
        auto [_begin, _end] = map.equal_range(it->first);
        _multimap_value_wrapper values(_begin, _end);

        // Treat the multiplicity as a member to distinguish it from the keys & values.
        // Also, multiplicities are similar to members since they are on the left side of values.
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

    if (it.is_ellipsis()) {
      output += es::op("...");

      if (last_line_length + get_length(output + " }") <= max_line_width) continue;

      shift_indent = true;
      goto rollback;
    }

    if constexpr (is_multimap<T>) {
      auto [_begin, _end] = map.equal_range(it->first);
      _multimap_value_wrapper values(_begin, _end);

      // Treat the multiplicity as a member to distinguish it from the keys & values.
      // Also, multiplicities are similar to members since they are on the left side of values.
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

template <typename T>
inline auto export_map(
    const T &map,
    const std::string &indent,
    size_t last_line_length,
    size_t current_depth,
    bool fail_on_newline
) -> std::enable_if_t<is_map<T> && !is_omitted_container<T>, std::string> {
  return export_map(omit_back() << map, indent, last_line_length, current_depth, fail_on_newline);
}

}  // namespace _detail

}  // namespace cpp_dump
