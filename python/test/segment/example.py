
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

import libnlp.segment as seg
import  os

print("jieba config**********")
print(seg.CONFIGS)
print(seg.CONFIGS_ROOT)
print(seg.DEFAULT_DICT_PATH)
print(seg.DEFAULT_STOP_WORDS_PATH)
print(seg.DEFAULT_USER_DICT_PATH)
print(seg.DEFAULT_IDF_PATH)
print(seg.DEFAULT_MODEL_PATH)
print("jieba config**********")

dictpath='jieba.dict.utf8'
modelpath='hmm_model.utf8'
userpath='user.dict.utf8'
idfpath='idf.utf8'
stopwords='stop_words.utf8'

dictpath = os.path.join(seg.CONFIGS_ROOT, dictpath)
modelpath = os.path.join(seg.CONFIGS_ROOT, modelpath)
userpath = os.path.join(seg.CONFIGS_ROOT, userpath)
idfpath = os.path.join(seg.CONFIGS_ROOT, idfpath)
stopwords = os.path.join(seg.CONFIGS_ROOT, stopwords)

jieba = seg.jieba(dictpath, modelpath, userpath, idfpath, stopwords)
print(jieba.cut_with_pos("是中国人",True)[0])
print(jieba.cut("我是中国人", True))
print(jieba.cut("我是中国人", False))
print(jieba.cut_with_pos("是中国人",True))
print(jieba.cut_with_pos("我是中国人",False))

#TODO add more cases

jieba = seg.default_jieba()
print(jieba.cut_with_pos("是中国人",True)[0])
print(jieba.cut("我是中国人", True))
print(jieba.cut("我是中国人", False))
print(jieba.cut_with_pos("是中国人",True))
print(jieba.cut_with_pos("我是中国人",False))
print(jieba.cut("我来到北京清华大学", False))
print(jieba.cut("乒乓球拍卖完了", False))
print(jieba.cut("中国科学技术大学", False))
print(jieba.cut("他来到了网易杭研大厦", False))