#!/bin/bash
set -e
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

mkdir _build
cd _build
cmake .. -DCMAKE_INSTALL_PREFIX=$PREFIX \
        -DCMAKE_BUILD_TYPE=Release \
        -DENABLE_TESTING=OFF \
        -DENABLE_BENCHARK=OFF \
        -DENABLE_PYTHON=OFF

cmake --build .
cmake --build . --target install