string(ASCII 27 esc0x1b)

function(diff_and_message log_file txt_file error_message)
    if(WIN32)
        string(REPLACE "/" "\\" log_file "${log_file}")
        string(REPLACE "/" "\\" txt_file "${txt_file}")
        execute_process(
            COMMAND fc.exe "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful OUTPUT_VARIABLE output
        )
    else()
        execute_process(COMMAND diff -u "${log_file}" "${txt_file}" RESULT_VARIABLE not_successful OUTPUT_VARIABLE output)
    endif()

    if(not_successful)
        if(WIN32)
            file(READ "${log_file}" output)
        endif()

        message(SEND_ERROR "${error_message}\n${output}")
    endif()
endfunction()

macro(crlf_to_lf var)
    string(REGEX REPLACE "\r\n" "\n" "${var}" "${${var}}")
endmacro()

macro(remove_es var)
    string(REGEX REPLACE "${esc0x1b}\\[[^m]*m" "" "${var}" "${${var}}")
endmacro()
