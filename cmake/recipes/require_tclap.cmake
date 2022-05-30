################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

find_path(TCLAP_INCLUDE_PATH NAMES tclap/CmdLine.h)
include_directories(${TCLAP_INCLUDE_PATH})
if((NOT TCLAP_INCLUDE_PATH))
    message(FATAL_ERROR "Fail to find tclap")
endif()
