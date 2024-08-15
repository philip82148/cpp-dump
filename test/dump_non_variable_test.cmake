if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

include("${test_dir}/common.cmake")

file(MAKE_DIRECTORY "${test_dir}/log")

set(log_file "${test_dir}/log/dump_non_variable.log")
set(txt_file "${test_dir}/txt/dump_non_variable.txt")

# no color
execute_process(
   COMMAND "${cmd_path}" 0 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
diff_and_message("${log_file}" "${txt_file}" "${log_file} does not match ${txt_file} !")

# original
execute_process(
   COMMAND "${cmd_path}" 1 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
remove_es(error_contents)
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} with color (original) does not match ${txt_file} !")

# by_syntax
execute_process(
   COMMAND "${cmd_path}" 2 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
remove_es(error_contents)
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} with color (by_syntax) does not match ${txt_file} !")
