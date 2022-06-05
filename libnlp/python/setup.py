################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

import os
import re
import subprocess
import sys
import warnings

import setuptools
import setuptools.command.build_ext
import wheel.bdist_wheel

_this_dir = os.path.dirname(os.path.abspath(__file__))
_clib_dir = os.path.join(_this_dir, 'python', 'libnlp','clib')
_build_dir = os.path.join(_this_dir, 'build', 'python')

_cmake_file = os.path.join(_this_dir, 'CMakeLists.txt')
_author_file = os.path.join(_this_dir, 'AUTHORS')
_readme_file = os.path.join(_this_dir, 'README.md')

try:
    sys.path.insert(0, os.path.join(_this_dir, 'python'))

    import libnlp.normalize_cc  # noqa
    _libnlpcc_built = True
except ImportError:
    _libnlpcc_built = False


def get_version_info():
    version_info = ['0', '3', '0']
    version = '.'.join(version_info)
    return version


def get_author_info():
    if not os.path.isfile(_author_file):
        return 'jeff.li', 'lijippy@163.com'

    authors = []
    emails = []
    author_pattern = re.compile(r'(.+) <(.+)>')
    with open(_author_file, 'rb') as f:
        for line in f:
            match = author_pattern.search(line.decode('utf-8'))
            if not match:
                continue
            authors.append(match.group(1))
            emails.append(match.group(2))

    if len(authors) == 0:
        return 'jeff.li', 'lijippy@163.comm'

    return ', '.join(authors), ', '.join(emails)

def get_long_description():
    with open(_readme_file, 'rb') as f:
        return f.read().decode('utf-8')


version_info = get_version_info()
author_info = get_author_info()

packages = ['libnlp', 'libnlp.normalize','libnlp.segment','libnlp.dict','libnlp.clib']


setuptools.setup(
    name='libnlp',
    version=version_info,
    author=author_info[0],
    author_email=author_info[1],
    description=" conversion between Traditional and Simplified Chinese",
    long_description=get_long_description(),
    long_description_content_type="text/markdown",
    url="https://github.com/lambda-search/libnlp",

    packages=packages,
    package_dir={'libnlp': 'python/libnlp'},
    package_data={str('libnlp'): [
        'clib/normalize_cc*',
        'clib/segment_cc*',
        'clib/dict_cc*',
        'clib/share/libnlp/normalize/*',
        'clib/share/libnlp/segment/jieba/*',
        'clib/share/libnlp/segment/jieba/pos_dict/*',
    ]},

    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Natural Language :: Chinese (Simplified)',
        'Natural Language :: Chinese (Traditional)',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: Apache Software License',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Localization',
        'Topic :: Text Processing :: Linguistic',
    ],
    license='Apache License 2.0',
    keywords=['libnlp', 'nlp', 'normalize', 'convert', 'chinese']
)
