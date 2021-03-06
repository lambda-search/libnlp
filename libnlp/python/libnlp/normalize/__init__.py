################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

from __future__ import absolute_import, unicode_literals

import os
import sys

from libnlp.clib.normalize_cc import _Xstring as xstring
from libnlp.clib import normalize_cc
__all__ = ['nlpcc', 'CONFIGS', '__version__', 'xstring']
__version__ = normalize_cc.__version__

_thisdir = os.path.dirname(os.path.abspath(__file__))
# now _thisdir = $BASE/libnlp/normalize/cc
# we need dir thisdir = $BASE/libnlp/clib/share/libnlp/normalize/cc
_thisdir = os.path.abspath(os.path.join(_thisdir, '..'))
_nlpcc_share_dir = os.path.join(_thisdir, 'clib', 'share', 'libnlp','normalize')

if sys.version_info.major == 2:
    text_type = unicode  # noqa
else:
    text_type = str

if os.path.isdir(_nlpcc_share_dir):
    CONFIGS = [f for f in os.listdir(_nlpcc_share_dir) if f.endswith('.json')]
else:
    CONFIGS = []


def _append_path_to_env(name, path):
    value = os.environ.get(name, '')
    if path in value:  # Path already exists
        return
    if value == '':
        value = path
    else:
        value += ':' + path
    os.environ[name] = value


class nlpcc(normalize_cc._NlpCC):

    def __init__(self, config='t2s'):
        if not config.endswith('.json'):
            config += '.json'
        if not os.path.isfile(config):
            config = os.path.join(_nlpcc_share_dir, config)
        super(nlpcc, self).__init__(config)
        self.config = config

    def convert(self, text):
        if isinstance(text, text_type):
            text = text.encode('utf-8')
        return super(nlpcc, self).convert(text, len(text))
