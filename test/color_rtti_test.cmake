if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

include("${test_dir}/common.cmake")

file(MAKE_DIRECTORY "${test_dir}/log")

set(log_file "${test_dir}/log/color_rtti.log")
set(txt_file "${test_dir}/txt/color_rtti.txt")

execute_process(
   COMMAND "${cmd_path}" ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "struct${esc0x1b}\\[0m${esc0x1b}\\[38;2;139;191;139m ${esc0x1b}\\[0m${esc0x1b}\\[32m" "" error_contents "${error_contents}")
string(REGEX REPLACE "${esc0x1b}\\[38;2;181;206;168m[^${esc0x1b}]*${esc0x1b}" "${esc0x1b}[38;2;181;206;168m314159265358979${esc0x1b}" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} does not match ${txt_file} !")
