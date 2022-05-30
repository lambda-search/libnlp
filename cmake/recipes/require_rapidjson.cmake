################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

find_path(RAPIDJSON_INCLUDE_PATH NAMES rapidjson/document.h)
include_directories(${RAPIDJSON_INCLUDE_PATH})
if((NOT RAPIDJSON_INCLUDE_PATH))
    message(FATAL_ERROR "Fail to find rapidjson")
endif()
