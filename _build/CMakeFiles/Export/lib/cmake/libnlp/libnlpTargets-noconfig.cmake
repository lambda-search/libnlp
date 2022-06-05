#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libnlp::libnlp" for configuration ""
set_property(TARGET libnlp::libnlp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(libnlp::libnlp PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liblibnlp.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/liblibnlp.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS libnlp::libnlp )
list(APPEND _IMPORT_CHECK_FILES_FOR_libnlp::libnlp "${_IMPORT_PREFIX}/lib/liblibnlp.dylib" )

# Import target "libnlp::cc_command_line" for configuration ""
set_property(TARGET libnlp::cc_command_line APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(libnlp::cc_command_line PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/cc_command_line"
  )

list(APPEND _IMPORT_CHECK_TARGETS libnlp::cc_command_line )
list(APPEND _IMPORT_CHECK_FILES_FOR_libnlp::cc_command_line "${_IMPORT_PREFIX}/bin/cc_command_line" )

# Import target "libnlp::cc_dict" for configuration ""
set_property(TARGET libnlp::cc_dict APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(libnlp::cc_dict PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/cc_dict"
  )

list(APPEND _IMPORT_CHECK_TARGETS libnlp::cc_dict )
list(APPEND _IMPORT_CHECK_FILES_FOR_libnlp::cc_dict "${_IMPORT_PREFIX}/bin/cc_dict" )

# Import target "libnlp::cc_phrase_extract" for configuration ""
set_property(TARGET libnlp::cc_phrase_extract APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(libnlp::cc_phrase_extract PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/cc_phrase_extract"
  )

list(APPEND _IMPORT_CHECK_TARGETS libnlp::cc_phrase_extract )
list(APPEND _IMPORT_CHECK_FILES_FOR_libnlp::cc_phrase_extract "${_IMPORT_PREFIX}/bin/cc_phrase_extract" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
