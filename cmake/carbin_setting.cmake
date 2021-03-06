################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

set (DIR_PREFIX ${CMAKE_INSTALL_PREFIX})
set (DIR_INCLUDE ${DIR_PREFIX}/include/)
set (DIR_SHARE ${DIR_PREFIX}/share/)
set (DIR_ETC ${DIR_PREFIX}/etc/)
set (DIR_LIBRARY ${DIR_PREFIX}/lib${LIB_SUFFIX}/)

if (DEFINED SHARE_INSTALL_PREFIX)
    set (DIR_SHARE ${SHARE_INSTALL_PREFIX})
endif (DEFINED SHARE_INSTALL_PREFIX)

if (DEFINED INCLUDE_INSTALL_DIR)
    set (DIR_INCLUDE ${INCLUDE_INSTALL_DIR})
endif (DEFINED INCLUDE_INSTALL_DIR)

if (DEFINED SYSCONF_INSTALL_DIR)
    set (DIR_ETC ${SYSCONF_INSTALL_DIR})
endif (DEFINED SYSCONF_INSTALL_DIR)

if (DEFINED LIB_INSTALL_DIR)
    set (DIR_LIBRARY ${LIB_INSTALL_DIR})
endif (DEFINED LIB_INSTALL_DIR)

set (DIR_SHARE_CC ${DIR_SHARE}/libnlp/normalize)
set (DIR_SHARE_LOCALE ${DIR_SHARE}/locale/)

set (DIR_SHARE_SEGMENT ${DIR_SHARE}/libnlp/segment)