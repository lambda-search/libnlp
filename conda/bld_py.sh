#!/bin/bash
set -e
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

$PYTHON setup.py build_ext bdist_wheel
whl=`ls dist`
pip${PY_VER} install --force-reinstall --prefix=$PREFIX dist/${whl}
rm -r build
rm -r dist