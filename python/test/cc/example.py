import libnlp.normalize.cc as cc
import libnlp.normalize.trimmer as trimmer

converter = cc.nlpcc('s2t.json')
print(converter.convert('汉字'))
print("jbh😬")
print(trimmer.trim_emoji("jbh😬"))