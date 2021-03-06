################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

if (NOT WIN32)
    string(ASCII 27 Esc)
    set(carbin_colour_reset   "${Esc}[m")
    set(carbin_colour_bold    "${Esc}[1m")
    set(carbin_red            "${Esc}[31m")
    set(carbin_green          "${Esc}[32m")
    set(carbin_yellow         "${Esc}[33m")
    set(carbin_blue           "${Esc}[34m")
    set(carbin_agenta         "${Esc}[35m")
    set(carbin_cyan           "${Esc}[36m")
    set(carbin_white          "${Esc}[37m")
    set(carbin_bold_red       "${Esc}[1;31m")
    set(carbin_bold_green     "${Esc}[1;32m")
    set(carbin_bold_yellow    "${Esc}[1;33m")
    set(carbin_bold_blue      "${Esc}[1;34m")
    set(carbin_bold_magenta   "${Esc}[1;35m")
    set(carbin_bold_cyan      "${Esc}[1;36m")
    set(carbin_bold_white     "${Esc}[1;37m")
endif ()