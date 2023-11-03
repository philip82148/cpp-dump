if(NOT source_dir)
   message(FATAL_ERROR "Variable source_dir not defined" )
endif()
if(NOT cmd_path)
   message(FATAL_ERROR "Variable cmd_path not defined" )
endif()
if(NOT cmd_args)
   message(FATAL_ERROR "Variable cmd_args not defined" )
endif()

file(MAKE_DIRECTORY "${source_dir}/log")

string(ASCII 27 esc)

list(GET cmd_args 0 suffix)
list(GET cmd_args 1 width)
list(GET cmd_args 2 depth)
set(log_file "${source_dir}/log/dump_${suffix}.log")
set(txt_file "${source_dir}/txt/dump_${suffix}.txt")

# no color
execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" 0 ERROR_FILE "${log_file}" COMMAND_ERROR_IS_FATAL ANY
)
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)
if(not_successful)
   message(SEND_ERROR "${log_file} does not match ${txt_file}!" )
endif()

# with color
execute_process(
   COMMAND "${cmd_path}" "${width}" "${depth}" 1 ERROR_VARIABLE error_contents COMMAND_ERROR_IS_FATAL ANY
)
string(REGEX REPLACE "${esc}\\[[^m]*m" "" error_contents "${error_contents}")
file(WRITE "${log_file}" "${error_contents}")
execute_process(
   COMMAND "${CMAKE_COMMAND}" -E compare_files "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful
)
if(not_successful)
   message(SEND_ERROR "${log_file} with color does not match ${txt_file}!" )
endif()
