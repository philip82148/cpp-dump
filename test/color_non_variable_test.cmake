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
list(GET cmd_args 1 es_style)
set(log_file "${test_dir}/log/color_non_variable_${suffix}.log")
set(txt_file "${test_dir}/txt/color_non_variable_${suffix}.txt")

execute_process(
   COMMAND "${cmd_path}" "${es_style}" ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
diff_and_message("${log_file}" "${txt_file}" "${log_file} does not match ${txt_file} !")
