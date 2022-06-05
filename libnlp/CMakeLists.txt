################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

FILE(GLOB_RECURSE BASE_SRC "base/*.cc")
FILE(GLOB_RECURSE DICT_SRC "dict/*.cc")
FILE(GLOB_RECURSE NORMALIZE_SRC "normalize/*.cc")
FILE(GLOB_RECURSE TRIE_SRC "trie/*.cc")
FILE(GLOB_RECURSE SEG_SRC "segment/*.cc")

configure_file(
        "${PROJECT_SOURCE_DIR}/libnlp/config/libnlp_config.h.in"
        "${PROJECT_BINARY_DIR}/libnlp/config/libnlp_config.h")

SET(LIBNLP_SRC
        ${BASE_SRC}
        ${DICT_SRC}
        ${NORMALIZE_SRC}
        ${TRIE_SRC}
        ${SEG_SRC}
        )

carbin_cc_library(
        NAMESPACE libnlp
        NAME libnlp
        SOURCES ${LIBNLP_SRC}
        PUBLIC_LINKED_TARGETS ${CAEBIN_DYLINK}
        PRIVATE_COMPILE_OPTIONS ${CARBIN_DEFAULT_COPTS}
        PUBLIC
        SHARED
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/libnlp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libnlp
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        )

install(FILES ${PROJECT_BINARY_DIR}/libnlp/config/libnlp_config.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libnlp/config
        )

carbin_cc_binary(
        NAMESPACE libnlp
        NAME cc_command_line
        SOURCES tools/cc_command_line.cc
        PUBLIC_LINKED_TARGETS ${CAEBIN_DYLINK} libnlp::libnlp
        PRIVATE_COMPILE_OPTIONS ${CARBIN_DEFAULT_COPTS}
        PUBLIC
)
carbin_cc_binary(
        NAMESPACE libnlp
        NAME cc_dict
        SOURCES tools/cc_dict_converter.cc
        PUBLIC_LINKED_TARGETS ${CAEBIN_DYLINK} libnlp::libnlp
        PRIVATE_COMPILE_OPTIONS ${CARBIN_DEFAULT_COPTS}
        PUBLIC
)

carbin_cc_binary(
        NAMESPACE libnlp
        NAME cc_phrase_extract
        SOURCES tools/cc_phrase_extract.cc
        PUBLIC_LINKED_TARGETS ${CAEBIN_DYLINK} libnlp::libnlp
        PRIVATE_COMPILE_OPTIONS ${CARBIN_DEFAULT_COPTS}
        PUBLIC
)

