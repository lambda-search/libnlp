#!/bin/sh
set -e

conda activate libnlp-dev

[ $? -ne 0 ] && conda env create -f conda/environment_linux.yaml && conda activate libnlp-dev

for VERSION in 2.7 3.5 3.6 3.7 3.8 3.9; do
    # Create and activate environment
    conda install python=$VERSION
    # Build and package
    pip install --no-cache-dir setuptools wheel
    python setup.py build_ext bdist_wheel \
        --plat-name manylinux1_x86_64
    rm -rf build python/libnlp/clib libnlp.egg-info
done

#conda install python=3.8
#python -m pip install twine
#python -m twine upload dist/*