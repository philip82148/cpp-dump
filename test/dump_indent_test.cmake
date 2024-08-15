if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

if(NOT cmd_args)
   message(FATAL_ERROR "Variable cmd_args not defined")
endif()

include("${test_dir}/common.cmake")

file(MAKE_DIRECTORY "${test_dir}/log")

list(GET cmd_args 0 suffix)
list(GET cmd_args 1 width)
list(GET cmd_args 2 depth)
set(log_file "${test_dir}/log/dump_indent_${suffix}.log")
set(txt_file "${test_dir}/txt/dump_indent_${suffix}.txt")

# no color
execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" 0 0 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
crlf_to_lf(error_contents)
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} does not match ${txt_file} !")

# original
execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" 1 0 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
remove_es(error_contents)
crlf_to_lf(error_contents)
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} with color (original) does not match ${txt_file} !")

# by_syntax
execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" 2 0 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
remove_es(error_contents)
crlf_to_lf(error_contents)
file(WRITE "${log_file}" "${error_contents}")
diff_and_message("${log_file}" "${txt_file}" "${log_file} with color (by_syntax) does not match ${txt_file} !")
