

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libnlp/python/libnlp/clib)
find_package(libnlp REQUIRED)

find_package(pybind11 CONFIG)
if (PKG_VERSION)
    add_definitions(
            -DNOR_VERSION="${PKG_VERSION}"
            -DSEG_VERSION="${PKG_VERSION}"
    )
endif (PKG_VERSION)
pybind11_add_module(normalize_cc normalize/py_normalize.cc)
target_link_libraries(normalize_cc PRIVATE libnlp::libnlp)

pybind11_add_module(segment_cc segment/py_segment.cc)
target_link_libraries(segment_cc PRIVATE libnlp::libnlp)
pybind11_add_module(dict_cc dict/py_dict.cc)
target_link_libraries(dict_cc PRIVATE libnlp::libnlp)

file(COPY   ${CMAKE_SOURCE_DIR}/${libnlp_ROOT}/share/libnlp
        DESTINATION ${PROJECT_BINARY_DIR}/libnlp/python/libnlp/clib/share
        FILES_MATCHING
        PATTERN "*"
        )

file(COPY setup.py DESTINATION ${PROJECT_BINARY_DIR}/libnlp)
file(COPY ${CMAKE_SOURCE_DIR}/README.md DESTINATION ${PROJECT_BINARY_DIR}/libnlp)
file(COPY ${PROJECT_SOURCE_DIR}/libnlp/python/libnlp
        DESTINATION ${PROJECT_BINARY_DIR}/libnlp/python
        FILES_MATCHING
        PATTERN "*.*"
        )
