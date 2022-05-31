
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/



#include "libnlp/base/unicode.h"
#include "libnlp/base/std_extension.h"
#include "gtest/gtest.h"


TEST(UnicodeTest, Test1) {
    std::string s = "你好世界";
    libnlp::rune_str_array runes;
    ASSERT_TRUE(decode_runes_in_string(s, runes));
    std::string actual;
    std::string expected = "[\"{\"rune\": \"20320\", \"offset\": 0, \"len\": 3}\", \"{\"rune\": \"22909\", \"offset\": 3, \"len\": 3}\", \"{\"rune\": \"19990\", \"offset\": 6, \"len\": 3}\", \"{\"rune\": \"30028\", \"offset\": 9, \"len\": 3}\"]";
    actual << runes;
    ASSERT_EQ(expected, actual);
}

TEST(UnicodeTest, Illegal) {
    std::string s = "123\x80";
    libnlp::rune_str_array runes;
    ASSERT_FALSE(decode_runes_in_string(s, runes));
    std::string actual;
    std::string expected = "[]";
    actual << runes;
    ASSERT_EQ(expected, actual);
}

TEST(UnicodeTest, Rand) {
    const size_t ITERATION = 1024;
    const size_t MAX_LEN = 256;
    std::string s;
    srand(time(NULL));

    for (size_t i = 0; i < ITERATION; i++) {
        size_t len = rand() % MAX_LEN;
        s.resize(len);
        for (size_t j = 0; j < len; j++) {
            s[rand() % len] = rand();
        }
        libnlp::rune_str_array runes;
        decode_runes_in_string(s, runes);
    }
}
