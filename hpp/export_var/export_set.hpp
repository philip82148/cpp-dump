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
struct _set_dummy_wrapper {
 public:
  _set_dummy_wrapper(const T &set) : _set(set) {}

  auto begin() const noexcept { return _set.begin(); }
  auto end() const noexcept { return _set.end(); }
  auto size() const noexcept { return _set.size(); }

 private:
  const T &_set;
};

template <typename T>
struct _multiset_wrapper {
 public:
  _multiset_wrapper(const T &set) : _begin(set, set.begin()), _end(set, set.end()) {}

  auto begin() const noexcept { return _begin; }
  auto end() const noexcept { return _end; }

 private:
  struct multiset_wrapper_iterator {
   public:
    using It = typename T::const_iterator;
    multiset_wrapper_iterator(const T &set, It it) : _set(set), _it(it) {}

    const auto &operator*() const noexcept { return *_it; }
    bool operator!=(const multiset_wrapper_iterator &to) const noexcept { return _it != to._it; }
    multiset_wrapper_iterator &operator++() {
      _it = _set.equal_range(*_it).second;
      return *this;
    }

   private:
    const T &_set;
    It _it;
  };

  multiset_wrapper_iterator _begin;
  multiset_wrapper_iterator _end;
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

  if (shift_indent && fail_on_newline) return "\n";

  std::string new_indent = indent + "  ";
  std::size_t next_depth = current_depth + 1;
  const auto &next_command = command.next();

  auto set_wrapper = ([&]() {
    if constexpr (is_multiset<T>) {
      return _multiset_wrapper(set);
    } else {
      // The wrapper is to avoid calling the copy constructor.
      return _set_dummy_wrapper(set);
    }
  })();
  auto skipped_set = command.create_skip_container(set_wrapper);

rollback:
  std::string output = es::bracket("{ ", current_depth);
  bool is_first = true;

  for (const auto &[skip, it, _] : skipped_set) {
    const auto &elem = *it;

    if (is_first) {
      is_first = false;
    } else {
      output += es::op(", ");
    }

    if (shift_indent) {
      if (skip) {
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

    if (skip) {
      output += es::op("...");

      if (last_line_length + get_length(output) + std::string_view(" }").size() <= max_line_width)
        continue;

      shift_indent = true;
      goto rollback;
    }

    std::string elem_str = export_var(
        elem, indent, last_line_length + get_length(output), next_depth, true, next_command
    );

    // Treat the multiplicity as a member as export_map() does.
    if constexpr (is_multiset<T>)
      elem_str += es::member(" (" + std::to_string(set.count(elem)) + ")");

    if (!has_newline(elem_str)) {
      output += elem_str;

      if (last_line_length + get_length(output) + std::string_view(" }").size() <= max_line_width)
        continue;
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
