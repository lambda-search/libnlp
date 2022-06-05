#!/bin/bash
set -e
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################



set -e

cmake -B _build_python_${PY_VER} \
      -Dlibnlp_ROOT=_libnlp_stage/ \
      -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_CCLIB=OFF \
      -DENABLE_PYTHON=ON
      .

WORKERS=`awk 'BEGIN{printf "%d\n",'$CPU_COUNT'/2}'`

make -C _build_python_${PY_VER} -j $WORKERS

cd _build_python_${PY_VER}/libnlp
$PYTHON setup.py install --single-version-externally-managed --record=record.txt --prefix=$PREFIX