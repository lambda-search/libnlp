#!/bin/bash
set -e
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

cmake -B _build \
        -DCMAKE_INSTALL_PREFIX=$PREFIX \
        -DCMAKE_BUILD_TYPE=Release \
        -DENABLE_TESTING=OFF \
        -DENABLE_BENCHMARK=OFF \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DENABLE_PYTHON=OFF .

WORKERS=`awk 'BEGIN{printf "%d\n",'$CPU_COUNT'/2}'`

make -C _build -j $WORKERS
cmake --install _build --prefix $PREFIX
cmake --install _build --prefix _libnlp_stage/