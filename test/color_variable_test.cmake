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

list(GET cmd_args 0 suffix)
list(GET cmd_args 1 es_style)

# os-dependent
if(UNIX AND NOT APPLE)
   set(log_file "${test_dir}/log/color_variable_linux_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_linux_${suffix}.txt")
elseif(APPLE)
   set(log_file "${test_dir}/log/color_variable_apple_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_apple_${suffix}.txt")
else()
   set(log_file "${test_dir}/log/color_variable_win32_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_win32_${suffix}.txt")
endif()

execute_process(
   COMMAND "${cmd_path}" 0 "${es_style}" 1 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()

# compiler-dependent
if(compiler_id MATCHES "GNU")
   set(log_file "${test_dir}/log/color_variable_gnu_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_gnu_${suffix}.txt")
elseif(compiler_id MATCHES "MSVC")
   set(log_file "${test_dir}/log/color_variable_msvc_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_msvc_${suffix}.txt")
else()
   set(log_file "${test_dir}/log/color_variable_clang_${suffix}.log")
   set(txt_file "${test_dir}/txt/color_variable_clang_${suffix}.txt")
endif()

execute_process(
   COMMAND "${cmd_path}" 1 "${es_style}" 1 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
