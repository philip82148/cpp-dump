if(NOT test_dir)
   message(FATAL_ERROR "Variable test_dir not defined")
endif()

if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined")
endif()

if(NOT cmd_args)
   message(FATAL_ERROR "Variable cmd_args not defined")
endif()

file(MAKE_DIRECTORY "${test_dir}/log")

string(ASCII 27 esc)

list(GET cmd_args 0 suffix)
list(GET cmd_args 1 width)
list(GET cmd_args 2 depth)
list(GET cmd_args 3 es_style)
set(log_file "${test_dir}/log/color_non_variable_${suffix}.log")
set(txt_file "${test_dir}/txt/color_non_variable_${suffix}.txt")

execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" "${es_style}" ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)

if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file} !")
   file(READ "${log_file}" contents)
   message(STATUS "${contents}")
endif()
