/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <string>
#include <type_traits>

#include "../escape_sequence.hpp"
#include "../export_command/export_command.hpp"
#include "../type_check.hpp"
#include "../utility.hpp"

namespace cpp_dump {

extern inline std::size_t max_line_width;

extern inline std::size_t max_depth;

namespace _detail {

template <typename T>
std::string
export_var(const T &, const std::string &, std::size_t, std::size_t, bool, const export_command &);

template <typename T>
struct _set_wrapper {
 public:
  _set_wrapper(const T &set) : _begin(set, set.begin()), _end(set, set.end()) {}
  _set_wrapper() = delete;

  auto begin() const { return _begin; }
  auto end() const { return _end; }

 private:
  struct set_wrapper_iterator {
   public:
    using It = typename T::const_iterator;
    set_wrapper_iterator(const T &set, It it) : set(set), it(it) {}
    set_wrapper_iterator() = delete;

    auto operator*() const { return *it; }
    bool operator!=(const set_wrapper_iterator &to) const { return it != to.it; }
    set_wrapper_iterator &operator++() {
      it = set.equal_range(*it).second;
      return *this;
    }

   private:
    const T &set;
    It it;
  };

  set_wrapper_iterator _begin;
  set_wrapper_iterator _end;
};

template <typename T>
inline auto export_set(
    const T &set,
    const std::string &indent,
    std::size_t last_line_length,
    std::size_t current_depth,
    bool fail_on_newline,
    const export_command &command
) -> std::enable_if_t<is_set<T>, std::string> {
  if (set.empty()) return es::bracket("{ }", current_depth);

  if (current_depth >= max_depth)
    return es::bracket("{ ", current_depth) + es::op("...") + es::bracket(" }", current_depth);

  bool shift_indent = is_iterable_like<iterable_elem_type<T>>;
  // 中身がiterable_likeでも常に長さに応じて改行するかどうかを決める場合は次
  // bool shift_indent = false;

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  std::size_t next_depth = current_depth + 1;
  auto next_command      = command.next();

  _set_wrapper set_wrapper(set);
  auto omitted_set = command.get_omitted_container(set_wrapper);

rollback:
  std::string output = es::bracket("{ ", current_depth);
  bool is_first      = true;

  for (const auto &[is_ellipsis, elem] : omitted_set) {
    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) {
      if (is_ellipsis) {
        output += "\n" + new_indent + es::op("...");
        continue;
      }

      output +=
          "\n" + new_indent
          + export_var(elem, new_indent, new_indent.length(), next_depth, false, next_command);

      // Treat the multiplicity as a member as export_map() does.
      if constexpr (is_multiset<T>)
        output += es::member(" (" + std::to_string(set.count(elem)) + ")");

      continue;
    }

    if (is_ellipsis) {
      output += es::op("...");

      if (last_line_length + get_length(output + " }") <= max_line_width) continue;

      shift_indent = true;
      goto rollback;
    }

    std::string elem_string = export_var(
        elem, indent, last_line_length + get_length(output), next_depth, true, next_command
    );

    // Treat the multiplicity as a member as export_map() does.
    if constexpr (is_multiset<T>)
      elem_string += es::member(" (" + std::to_string(set.count(elem)) + ")");

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
