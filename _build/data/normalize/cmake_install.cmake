# Install script for directory: /Users/liyinbin/github/lambda-search/libnlp/data/normalize

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/STCharacters.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/STPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TSCharacters.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TSPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TWVariants.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TWVariantsRevPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/HKVariants.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/HKVariantsRevPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/JPVariants.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/JPShinjitaiCharacters.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/JPShinjitaiPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TWPhrases.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TWPhrasesRev.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/TWVariantsRev.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/HKVariantsRev.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES "/Users/liyinbin/github/lambda-search/libnlp/_build/data/normalize/JPVariantsRev.ocd2")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libnlp/normalize" TYPE FILE FILES
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/hk2s.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/hk2t.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/jp2t.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/s2hk.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/s2t.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/s2tw.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/s2twp.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/t2hk.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/t2jp.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/t2s.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/t2tw.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/tw2s.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/tw2sp.json"
    "/Users/liyinbin/github/lambda-search/libnlp/data/normalize/config/tw2t.json"
    )
endif()

