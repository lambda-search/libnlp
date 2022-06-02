
################################################################
#
# Copyright (c) 2022, liyinbin
# All rights reserved.
# Author by liyibin (jeff.li)
#
#################################################################

import libnlp.dict as dt

builder = dt.dict_builder("111")

builder.add("BYVoid", "byv")

builder.add("zigzagzig", "zag")
builder.add("積羽沉舟", "羣輕折軸")
builder.add("清", "Tsing")
builder.add("清華", "Tsinghua")
builder.add("清華大學", "TsinghuaUniversity")

dicter = builder.get_dict("text")
print(dicter)

en = dicter.match("清")
print(en)
print("dict_entity")
print("key: ", en.key())
print("values: ", en.values())
print("get_default: ", en.get_default())
print("num_values: ", en.num_values())
print("key_length: ", en.key_length())
print("values[0]: ", en.values()[en.num_values()-1])
print("dict_entity end")
print(dicter.match("清華"))
print(dicter.match("清華"))

print(dicter.match("積羽沉舟"))


