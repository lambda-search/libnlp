#!/bin/bash
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################
set -e

export CONDA_BLD_PATH=build

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build
conda build .