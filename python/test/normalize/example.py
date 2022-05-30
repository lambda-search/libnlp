
import libnlp.normalize as nor

converter = nor.nlpcc('s2t.json')
print("simple to convert", converter.convert('汉字'))
print('enoji: ', "jbh😬")

s = nor.xstring('汉字😬', nor.xstring.utf8_t)
print('chinese size: ', s.size())
print('raw byte length: ', s.byte_length())
print('raw data: ', s.data())
print('print object: ', s)

print("after trim_emoji")
#s = nor.xstring('汉字', nor.xstring.utf8_t)
s.trim_emoji()
print('chinese size: ',s.size())
print('raw byte length: ',s.byte_length())
print('raw data: ',s.data())
print('print object: ',s)
print("version: ", nor.__version__)