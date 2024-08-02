if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

set(log_file "${test_dir}/log/color_rtti.log")
set(txt_file "${test_dir}/txt/color_rtti.txt")

execute_process(
   COMMAND "${cmd_path}" ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "struct " "" error_contents "${error_contents}")
string(REGEX REPLACE "${esc}\\[38;2;181;206;168m[^${esc}]*${esc}" "${esc}[38;2;181;206;168m314159265358979${esc}" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
