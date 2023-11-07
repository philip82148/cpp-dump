/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>

namespace cpp_dump {

using log_info_func_t = std::function<std::string(const std::string &, int, const std::string &)>;

namespace log_info {

inline std::string default_func(const std::string &, int, const std::string &) { return "[dump] "; }

inline log_info_func_t line(int min_width = 0, bool show_func = false) {
  return [=](const std::string &, int line_no, const std::string &func_name) -> std::string {
    std::ostringstream ss;
    ss << std::left << std::setw(min_width);

    if (show_func) {
      ss << ":" + std::to_string(line_no) + " (" + func_name + ")";
    } else {
      ss << ":" + std::to_string(line_no);
    }

    return "[" + ss.str() + "] ";
  };
}

inline log_info_func_t basename(bool show_func = false) {
  return
      [=](const std::string &fullpath, int line_no, const std::string &func_name) -> std::string {
        auto slash_pos = fullpath.find_last_of("/\\");
        if (slash_pos == std::string::npos) slash_pos = 0;
        else ++slash_pos;
        std::string filename = fullpath.substr(slash_pos);

        auto dot_pos = filename.rfind('.');
        if (dot_pos == std::string::npos) dot_pos = filename.length();
        std::string basename = filename.substr(0, dot_pos);

        std::string output = basename + ":" + std::to_string(line_no);
        if (show_func) output += " (" + func_name + ")";

        return "[" + output + "] ";
      };
}

inline log_info_func_t filename(bool show_func = false) {
  return
      [=](const std::string &fullpath, int line_no, const std::string &func_name) -> std::string {
        auto slash_pos = fullpath.find_last_of("/\\");
        if (slash_pos == std::string::npos) slash_pos = 0;
        else ++slash_pos;
        std::string filename = fullpath.substr(slash_pos);

        std::string output = filename + ":" + std::to_string(line_no);
        if (show_func) output += " (" + func_name + ")";

        return "[" + output + "] ";
      };
}

inline log_info_func_t filepath(int substr_start = 0, bool show_func = false) {
  return
      [=](const std::string &fullpath, int line_no, const std::string &func_name) -> std::string {
        std::string output = fullpath.substr(substr_start) + ":" + std::to_string(line_no);
        if (substr_start > 0) output = "..." + output;
        if (show_func) output += " (" + func_name + ")";

        return "[" + output + "] ";
      };
}

inline log_info_func_t fixed_length(int min_width = 0, int max_width = 0, bool show_func = false) {
  return
      [=](const std::string &fullpath, int line_no, const std::string &func_name) -> std::string {
        std::ostringstream ss;
        ss << std::left << std::setw(min_width);

        if (show_func) {
          ss << fullpath + ":" + std::to_string(line_no) + " (" + func_name + ")";
        } else {
          ss << fullpath + ":" + std::to_string(line_no);
        }

        std::string output = ss.str();

        if (max_width > 0 && output.length() > static_cast<std::size_t>(max_width))
          output = "..." + output.substr(output.length() - max_width + 3);

        return "[" + output + "] ";
      };
}

}  // namespace log_info

}  // namespace cpp_dump
