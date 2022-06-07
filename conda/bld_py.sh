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
      -DENABLE_PYTHON=ON \
      .


make -C _build_python_${PY_VER}

cd _build_python_${PY_VER}/libnlp
$PYTHON setup.py install --single-version-externally-managed --record=record.txt --prefix=$PREFIX