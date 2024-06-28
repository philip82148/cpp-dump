if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

if(NOT compiler_id)
   message(FATAL_ERROR "Variable compiler_id not defined")
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

# os-dependent
if(UNIX AND NOT APPLE)
   set(log_file "${test_dir}/log/dump_variable_linux.log")
   set(txt_file "${test_dir}/txt/dump_variable_linux.txt")
elseif(APPLE)
   set(log_file "${test_dir}/log/dump_variable_apple.log")
   set(txt_file "${test_dir}/txt/dump_variable_apple.txt")
else()
   set(log_file "${test_dir}/log/dump_variable_win32.log")
   set(txt_file "${test_dir}/txt/dump_variable_win32.txt")
endif()

# no color
execute_process(
   COMMAND "${cmd_path}" 0 0 0 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()

# with color
execute_process(
   COMMAND "${cmd_path}" 0 1 0 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "${esc}\\[[^m]*m" "" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()

# compiler-dependent
if(compiler_id MATCHES "GNU")
   set(log_file "${test_dir}/log/dump_variable_gnu.log")
   set(txt_file "${test_dir}/txt/dump_variable_gnu.txt")
elseif(compiler_id MATCHES "MSVC")
   set(log_file "${test_dir}/log/dump_variable_msvc.log")
   set(txt_file "${test_dir}/txt/dump_variable_msvc.txt")
else()
   set(log_file "${test_dir}/log/dump_variable_clang.log")
   set(txt_file "${test_dir}/txt/dump_variable_clang.txt")
endif()

# no color
execute_process(
   COMMAND "${cmd_path}" 1 0 0 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()

# with color
execute_process(
   COMMAND "${cmd_path}" 1 1 0 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "${esc}\\[[^m]*m" "" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
