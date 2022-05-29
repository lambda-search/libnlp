import libnlp.cc as cc
import os
converter = cc.nlpcc('s2t.json')
print(converter.convert('汉字'))