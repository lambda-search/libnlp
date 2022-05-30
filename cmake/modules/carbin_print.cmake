################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

include(carbin_color)

function(carbin_debug)
    if (CARBIN_STATUS_DEBUG)
        string(TIMESTAMP timestamp)
        if (CARBIN_CACHE_RUN)
            set(type "DEBUG (CACHE RUN)")
        else ()
            set(type "DEBUG")
        endif ()
        message(STATUS "${carbin_blue}[carbin *** ${type} *** ${timestamp}] ${ARGV}${carbin_colour_reset}")
    endif ()
endfunction(carbin_debug)

function(carbin_print)
    if (CARBIN_STATUS_PRINT OR CARBIN_STATUS_DEBUG)
        if (CARBIN_CACHE_RUN)
            carbin_debug("${ARGV}")
        else ()
            message(STATUS "${carbin_green}[carbin] ${ARGV}${carbin_colour_reset}")
        endif ()
    endif ()
endfunction(carbin_print)

function(carbin_error)
    message("")
    foreach (print_message ${ARGV})
        message(SEND_ERROR "${carbin_bold_red}[carbin ** INTERNAL **] ${print_message}${carbin_colour_reset}")
    endforeach ()
    message(FATAL_ERROR "${carbin_bold_red}[carbin ** INTERNAL **] [Directory:${CMAKE_CURRENT_LIST_DIR}]${carbin_colour_reset}")
    message("")
endfunction(carbin_error)

function(carbin_warn)
    message("")
    foreach (print_message ${ARGV})
        message(WARNING "${carbin_red}[carbin WARNING] ${print_message}${carbin_colour_reset}")
    endforeach ()
    message(WARNING "${carbin_red}[carbin WARNING] [Directory:${CMAKE_CURRENT_LIST_DIR}]${carbin_colour_reset}")
    message("")
endfunction(carbin_warn)

set(CARBIN_ALIGN_LENGTH 30)
MACRO(carbin_print_label Label Value)
    string(LENGTH ${Label} lLength)
    math(EXPR paddingLeng ${CARBIN_ALIGN_LENGTH}-${lLength})
    string(REPEAT " "  ${paddingLeng} PADDING)
    message("${carbin_yellow}${Label}${carbin_colour_reset}:${PADDING}${carbin_cyan}${Value}${carbin_colour_reset}")
ENDMACRO()

MACRO(carbin_raw Value)
    message("${Value}")
ENDMACRO()
