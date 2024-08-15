string(ASCII 27 esc0x1b)

function(diff_and_message log_file txt_file error_message)
    if(WIN32)
        execute_process(COMMAND diff "(cat ${log_file})" "(cat ${txt_file})" RESULT_VARIABLE not_successful OUTPUT_VARIABLE output)
    else()
        execute_process(COMMAND diff -u "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful OUTPUT_VARIABLE output)
    endif()

    if(not_successful)
        message(SEND_ERROR "${error_message}\n${output}")
    endif()
endfunction()
