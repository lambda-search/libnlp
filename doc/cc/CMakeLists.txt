if (BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if (NOT DOXYGEN_FOUND)
        message(
                FATAL_ERROR
                "Doxygen is needed to build the documentation. Please install it correctly"
        )
    endif ()

    configure_file(
            libnlp_cc.doxy.in
            libnlp_cc.doxy
            @ONLY
            IMMEDIATE
    )

    add_custom_target(
            apidoc
            ALL
            COMMENT
            "Building API Documentation"
            COMMAND
            doxygen ${PROJECT_BINARY_DIR}/doc/libnlp_cc.doxy
            SOURCES
            ${PROJECT_BINARY_DIR}/doc/libnlp_cc.doxy
    )

    install(
            DIRECTORY
            ${CMAKE_BINARY_DIR}/doc/html
            DESTINATION
            ${DIR_SHARE_CC}/doc
    )

    set_directory_properties(
            PROPERTIES
            ADDITIONAL_MAKE_CLEAN_FILES
            "${CMAKE_BINARY_DIR}/doc/html"
    )
endif ()
