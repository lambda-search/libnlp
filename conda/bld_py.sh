#!/bin/bash
set -e

$PYTHON setup.py build_ext bdist_wheel
whl=`ls dist`
pip${PY_VER} install --force-reinstall --prefix=$PREFIX dist/${whl}
rm -r build
rm -r dist