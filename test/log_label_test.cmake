if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined" )
endif()
if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined" )
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

set(log_file "${test_dir}/log/log_label_test.log")
set(txt_file "${test_dir}/txt/log_label_test.txt")

# no color
execute_process(
   COMMAND "${cmd_path}" 0 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)
if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file}!" )
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()

# with color
execute_process(
   COMMAND "${cmd_path}" 1 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "${esc}\\[[^m]*m" "" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)
if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file}!" )
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
