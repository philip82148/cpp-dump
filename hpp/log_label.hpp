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

namespace log_label {

using log_label_func_t =
    std::function<std::string(const std::string &, std::size_t, const std::string &)>;

inline std::string default_func(const std::string &, std::size_t, const std::string &) {
  return "[dump] ";
}

inline log_label_func_t line(int min_width = 0, bool show_func = false) {
  return [=](const std::string &, std::size_t line, const std::string &func_name) -> std::string {
    std::ostringstream ss;
    ss << std::left << std::setw(min_width);

    if (show_func) {
      ss << "[:" + std::to_string(line) + " (" + func_name + ")] ";
    } else {
      ss << "[:" + std::to_string(line) + "] ";
    }

    return ss.str();
  };
}

inline log_label_func_t basename(int min_width = 0, bool show_func = false) {
  return [=](const std::string &fullpath, std::size_t line, const std::string &func_name
         ) -> std::string {
    auto slash_pos = fullpath.find_last_of("/\\");
    if (slash_pos == std::string::npos) slash_pos = 0;
    else ++slash_pos;
    std::string filename = fullpath.substr(slash_pos);

    auto dot_pos = filename.rfind('.');
    if (dot_pos == std::string::npos) dot_pos = filename.length();
    std::string basename = filename.substr(0, dot_pos);

    std::ostringstream ss;
    ss << std::left << std::setw(min_width);

    if (show_func) {
      ss << "[" + basename + ":" + std::to_string(line) + " (" + func_name + ")] ";
    } else {
      ss << "[" + basename + ":" + std::to_string(line) + "] ";
    }

    return ss.str();
  };
}

inline log_label_func_t filename(int min_width = 0, bool show_func = false) {
  return [=](const std::string &fullpath, std::size_t line, const std::string &func_name
         ) -> std::string {
    auto slash_pos = fullpath.find_last_of("/\\");
    if (slash_pos == std::string::npos) slash_pos = 0;
    else ++slash_pos;
    std::string filename = fullpath.substr(slash_pos);

    std::ostringstream ss;
    ss << std::left << std::setw(min_width);

    if (show_func) {
      ss << "[" + filename + ":" + std::to_string(line) + " (" + func_name + ")] ";
    } else {
      ss << "[" + filename + ":" + std::to_string(line) + "] ";
    }

    return ss.str();
  };
}

inline log_label_func_t fullpath(int min_width = 0, int substr_start = 0, bool show_func = false) {
  return [=](const std::string &fullpath, std::size_t line, const std::string &func_name
         ) -> std::string {
    std::ostringstream ss;
    ss << std::left << std::setw(min_width);

    if (show_func) {
      ss << "[" + fullpath.substr(substr_start) + ":" + std::to_string(line) + " (" + func_name
                + ")] ";
    } else {
      ss << "[" + fullpath.substr(substr_start) + ":" + std::to_string(line) + "] ";
    }

    return ss.str();
  };
}

inline log_label_func_t fixed_length(int width = 0, int substr_start = 0, bool show_func = false) {
  return [=](const std::string &fullpath, std::size_t line, const std::string &func_name
         ) -> std::string {
    std::ostringstream ss;
    ss << std::left << std::setw(width);

    if (show_func) {
      ss << "[" + fullpath.substr(substr_start) + ":" + std::to_string(line) + " (" + func_name
                + ")] ";
    } else {
      ss << "[" + fullpath.substr(substr_start) + ":" + std::to_string(line) + "] ";
    }

    std::string output = ss.str();

    if (width > 0 && output.length() > static_cast<std::size_t>(width))
      output = "[.. " + output.substr(output.length() - width + 4);

    return output;
  };
}

}  // namespace log_label

}  // namespace cpp_dump
