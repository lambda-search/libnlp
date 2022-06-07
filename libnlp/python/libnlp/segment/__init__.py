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

from libnlp.clib.segment_cc import _WordType as jieba_word
from libnlp.clib.segment_cc import _JiebaPY as jieba
from libnlp.clib import segment_cc

__all__ = ['CONFIGS', 'CONFIGS_ROOT', '__version__', 'jieba', 'jieba_word',
           'DEFAULT_DICT_PATH',
           'DEFAULT_MODEL_PATH',
           'DEFAULT_USER_DICT_PATH',
           'DEFAULT_IDF_PATH',
           'DEFAULT_STOP_WORDS_PATH']
__version__ = segment_cc.__version__

_thisdir = os.path.dirname(os.path.abspath(__file__))
# now _thisdir = $BASE/libnlp/normalize/cc
# we need dir thisdir = $BASE/libnlp/clib/share/libnlp/normalize/cc
_thisdir = os.path.abspath(os.path.join(_thisdir, '..'))
_jieba_share_dir = os.path.join(_thisdir, 'clib', 'share', 'libnlp', 'segment', 'jieba')

if sys.version_info.major == 2:
    text_type = unicode  # noqa
else:
    text_type = str

if os.path.isdir(_jieba_share_dir):
    CONFIGS_ROOT = _jieba_share_dir
    CONFIGS = [f for f in os.listdir(_jieba_share_dir) if f.endswith('.utf8')]
else:
    CONFIGS_ROOT = ''
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


_dict_file = 'jieba.dict.utf8'
_model_file = 'hmm_model.utf8'
_user_dict_file = 'user.dict.utf8'
_idf_file = 'idf.utf8'
_stop_words_file = 'stop_words.utf8'

DEFAULT_DICT_PATH = os.path.join(CONFIGS_ROOT, _dict_file)
DEFAULT_MODEL_PATH = os.path.join(CONFIGS_ROOT, _model_file)
DEFAULT_USER_DICT_PATH = os.path.join(CONFIGS_ROOT, _user_dict_file)
DEFAULT_IDF_PATH = os.path.join(CONFIGS_ROOT, _idf_file)
DEFAULT_STOP_WORDS_PATH = os.path.join(CONFIGS_ROOT, _stop_words_file)


def default_jieba():
    return jieba(DEFAULT_DICT_PATH, DEFAULT_MODEL_PATH, DEFAULT_USER_DICT_PATH, DEFAULT_IDF_PATH,
                 DEFAULT_STOP_WORDS_PATH)
