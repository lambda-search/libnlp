

FILE(GLOB_RECURSE BASE_SRC "base/*.cc")
FILE(GLOB_RECURSE DICT_SRC "dict/*.cc")
FILE(GLOB_RECURSE NORMALIZE_SRC "normalize/*.cc")
FILE(GLOB_RECURSE TRIE_SRC "trie/*.cc")

configure_file(
        "${PROJECT_SOURCE_DIR}/libnlp/config/libnlp_config.h.in"
        "${PROJECT_BINARY_DIR}/libnlp/config/libnlp_config.h")

SET(LIBNLP_SRC
        ${BASE_SRC}
        ${DICT_SRC}
        ${NORMALIZE_SRC}
        ${TRIE_SRC}
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

if(ENABLE_PYTHON)
    find_package(pybind11 CONFIG)
    pybind11_add_module(nlp_cc python/normalize/cc/py_libnlp_cc.cc)
    target_link_libraries(nlp_cc PRIVATE libnlp::libnlp)

    pybind11_add_module(trimmer python/normalize/trimmer/py_libnlp_trimmer.cc)
    target_link_libraries(trimmer PRIVATE libnlp::libnlp)

endif()