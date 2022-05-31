################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################


file(GLOB SRC "*.cc")

include(require_gtest)
include(require_gmock)
set(GTM_LIB
        ${GTEST_LIB}
        ${GMOCK_LIB}
        )

foreach (fl ${SRC})

    string(REGEX REPLACE ".+/(.+)\\.cc$" "\\1" TEST_NAME ${fl})
    get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    string(REPLACE " " "_" DIR_NAME ${DIR_NAME})

    set(EXE_NAME ${TEST_NAME})
    carbin_cc_test(
            NAME ${EXE_NAME}
            SOURCES ${fl}
            PUBLIC_LINKED_TARGETS ${GTEST_MAIN_LIB}  ${GTM_LIB} ${CARBIN_DYLINK} libnlp::libnlp
            PRIVATE_COMPILE_OPTIONS ${CARBIN_TEST_COPTS}
            VERBOSE
    )
endforeach (fl ${SRC})