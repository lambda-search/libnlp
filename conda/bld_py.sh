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

# Build swigfaiss.so/swigfaiss_avx2.so.
cmake -B _build_python_${PY_VER} \
      -Dlibnlp_ROOT=_liblibnlp_stage/ \
      -DCMAKE_BUILD_TYPE=Release \
      -DPython_EXECUTABLE=$PYTHON \
      -DENABLE_CCLIB=OFF \
      -DENABLE_PYTHON=ON
      .

WORKERS=`awk 'BEGIN{printf "%d\n",'$CPU_COUNT'/2}'`

make -C _build -j WORKERS

cd _build_python_${PY_VER}/libnlp
$PYTHON setup.py install --single-version-externally-managed --record=record.txt --prefix=$PREFIX