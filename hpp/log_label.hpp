/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <string_view>

namespace cpp_dump {

namespace log_label {

/**
 * Type of cpp_dump::log_label_func.
 */
using log_label_func_t =
    std::function<std::string(std::string_view, std::size_t, std::string_view)>;

/*
 * Default function assigned to cpp_dump::log_label_func.
 */
inline std::string default_func(std::string_view, std::size_t, std::string_view) {
  return "[dump] ";
}

/*
 * Function that create a function to assign to `cpp_dump::log_label_func`.
 * See README for details.
 */
inline log_label_func_t line(bool show_func = false, int min_width = 0) {
  return [=](std::string_view, std::size_t line, std::string_view func_name) -> std::string {
    std::string output = "[:" + std::to_string(line);
    if (show_func) output.append(" (").append(func_name).append(")");

    int space_len = (min_width - 2) - static_cast<int>(output.size());
    if (space_len > 0) output.append(space_len, ' ');

    output += "] ";
    return output;
  };
}

/*
 * Function that create a function to assign to `cpp_dump::log_label_func`.
 * See README for details.
 */
inline log_label_func_t basename(bool show_func = false, int min_width = 0) {
  return
      [=](std::string_view fullpath, std::size_t line, std::string_view func_name) -> std::string {
        auto slash_pos = fullpath.find_last_of("/\\");
        if (slash_pos == std::string::npos) slash_pos = 0;
        else ++slash_pos;
        std::string filename_(fullpath.substr(slash_pos));

        auto dot_pos = filename_.rfind('.');
        if (dot_pos == std::string::npos) dot_pos = filename_.length();
        std::string basename = filename_.substr(0, dot_pos);

        std::string output = "[" + basename + ":" + std::to_string(line);
        if (show_func) output.append(" (").append(func_name).append(")");

        int space_len = (min_width - 2) - static_cast<int>(output.size());
        if (space_len > 0) output.append(space_len, ' ');

        output += "] ";
        return output;
      };
}

/*
 * Function that create a function to assign to `cpp_dump::log_label_func`.
 * See README for details.
 */
inline log_label_func_t filename(bool show_func = false, int min_width = 0) {
  return
      [=](std::string_view fullpath, std::size_t line, std::string_view func_name) -> std::string {
        auto slash_pos = fullpath.find_last_of("/\\");
        if (slash_pos == std::string::npos) slash_pos = 0;
        else ++slash_pos;
        std::string filename(fullpath.substr(slash_pos));

        std::string output = "[" + filename + ":" + std::to_string(line);
        if (show_func) output.append(" (").append(func_name).append(")");

        int space_len = (min_width - 2) - static_cast<int>(output.size());
        if (space_len > 0) output.append(space_len, ' ');

        output += "] ";
        return output;
      };
}

/*
 * Function that create a function to assign to `cpp_dump::log_label_func`.
 * See README for details.
 */
inline log_label_func_t fullpath(int substr_start, bool show_func = false, int min_width = 0) {
  return
      [=](std::string_view fullpath, std::size_t line, std::string_view func_name) -> std::string {
        std::string output = "[";
        output.append(fullpath.substr(std::min<std::size_t>(substr_start, fullpath.length())));
        output.append(":").append(std::to_string(line));

        if (show_func) output.append(" (").append(func_name).append(")");

        int space_len = (min_width - 2) - static_cast<int>(output.size());
        if (space_len > 0) output.append(space_len, ' ');

        output += "] ";
        return output;
      };
}

/*
 * Function that create a function to assign to `cpp_dump::log_label_func`.
 * See README for details.
 */
inline log_label_func_t fixed_length(
    int min_width, int max_width, int substr_start, bool show_func = false
) {
  return
      [=](std::string_view fullpath, std::size_t line, std::string_view func_name) -> std::string {
        std::string output = "[";
        output.append(fullpath.substr(std::min<std::size_t>(substr_start, fullpath.length())));
        output.append(":").append(std::to_string(line));

        if (show_func) output.append(" (").append(func_name).append(")");

        int space_len = (min_width - 2) - static_cast<int>(output.size());
        if (space_len > 0) output.append(space_len, ' ');

        if (max_width > 2 && output.length() > static_cast<std::size_t>(max_width - 2))
          output = "[.. " + output.substr(output.length() - std::max(max_width - 6, 0));

        output += "] ";
        return output;
      };
}

}  // namespace log_label

}  // namespace cpp_dump
