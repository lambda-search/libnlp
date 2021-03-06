################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

include(GNUInstallDirs)

if(${PROJECT_NAME}_VERSION)
    set(CARBIN_SUBDIR "${PROJECT_NAME}_${PROJECT_VERSION}")
    set(CARBIN_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}/${CARBIN_SUBDIR}")
    set(CARBIN_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${CARBIN_SUBDIR}")
    set(CARBIN_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/{CARBIN_SUBDIR}")
    set(CARBIN_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/${CARBIN_SUBDIR}")
else()
    set(CARBIN_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
    set(CARBIN_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    set(CARBIN_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(CARBIN_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
endif()