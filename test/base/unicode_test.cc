
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/



#include "libnlp/base/unicode.h"
#include "libnlp/base/std_extension.h"
#include "gtest/gtest.h"

using namespace libnlp;
using namespace std;

TEST(UnicodeTest, Test1) {
    string s = "你好世界";
    rune_str_array runes;
    ASSERT_TRUE(decode_runes_in_string(s, runes));
    string actual;
    string expected = "[\"{\"rune\": \"20320\", \"offset\": 0, \"len\": 3}\", \"{\"rune\": \"22909\", \"offset\": 3, \"len\": 3}\", \"{\"rune\": \"19990\", \"offset\": 6, \"len\": 3}\", \"{\"rune\": \"30028\", \"offset\": 9, \"len\": 3}\"]";
    actual << runes;
    ASSERT_EQ(expected, actual);
}

TEST(UnicodeTest, Illegal) {
    string s = "123\x80";
    rune_str_array runes;
    ASSERT_FALSE(decode_runes_in_string(s, runes));
    string actual;
    string expected = "[]";
    actual << runes;
    ASSERT_EQ(expected, actual);
}

TEST(UnicodeTest, Rand) {
    const size_t ITERATION = 1024;
    const size_t MAX_LEN = 256;
    string s;
    srand(time(NULL));

    for (size_t i = 0; i < ITERATION; i++) {
        size_t len = rand() % MAX_LEN;
        s.resize(len);
        for (size_t j = 0; j < len; j++) {
            s[rand() % len] = rand();
        }
        rune_str_array runes;
        decode_runes_in_string(s, runes);
    }
}
