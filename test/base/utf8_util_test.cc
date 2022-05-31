
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/



#include "libnlp/base/utf8_util.h"
#include "test_utils.h"

namespace libnlp {

    class UTF8UtilTest : public ::testing::Test {
    protected:
        UTF8UtilTest() : text("東菄鶇䍶𠍀倲𩜍𢘐"), length(strlen(text)) {};
        const char *text;
        const size_t length;
    };

    TEST_F(UTF8UtilTest, NextCharLength) {
        EXPECT_EQ(3, UTF8Util::NextCharLength(text));
        EXPECT_EQ(3, UTF8Util::NextCharLength(text + 3));
        EXPECT_EQ(3, UTF8Util::NextCharLength(text + 6));
        EXPECT_EQ(3, UTF8Util::NextCharLength(text + 9));
        EXPECT_EQ(4, UTF8Util::NextCharLength(text + 12));
        EXPECT_EQ(3, UTF8Util::NextCharLength(text + 16));
        EXPECT_EQ(4, UTF8Util::NextCharLength(text + 19));
        EXPECT_EQ(4, UTF8Util::NextCharLength(text + 23));
        EXPECT_THROW(EXPECT_EQ(3, UTF8Util::NextCharLength(text + 1)), InvalidUTF8);
        EXPECT_THROW(EXPECT_EQ(3, UTF8Util::NextCharLength(text + 2)), InvalidUTF8);
    }

    TEST_F(UTF8UtilTest, PrevCharLength) {
        EXPECT_EQ(4, UTF8Util::PrevCharLength(text + length));
        EXPECT_EQ(4, UTF8Util::PrevCharLength(text + length - 4));
        EXPECT_EQ(3, UTF8Util::PrevCharLength(text + length - 8));
        EXPECT_THROW(EXPECT_EQ(3, UTF8Util::PrevCharLength(text + 1)), InvalidUTF8);
    }

    TEST_F(UTF8UtilTest, Length) {
        EXPECT_EQ(0, UTF8Util::Length(""));
        EXPECT_EQ(8, UTF8Util::Length(text));
    }

    TEST_F(UTF8UtilTest, NotShorterThan) {
        EXPECT_TRUE(UTF8Util::NotShorterThan(text, 0));
        EXPECT_TRUE(UTF8Util::NotShorterThan(text, length));
        EXPECT_FALSE(UTF8Util::NotShorterThan(text, length + 1));
    }

    TEST_F(UTF8UtilTest, TruncateUTF8) {
        EXPECT_EQ("", UTF8Util::TruncateUTF8(text, 0));
        EXPECT_EQ("", UTF8Util::TruncateUTF8(text, 1));
        EXPECT_EQ("東", UTF8Util::TruncateUTF8(text, 3));
        EXPECT_EQ("東", UTF8Util::TruncateUTF8(text, 4));
        EXPECT_EQ("東菄鶇䍶𠍀", UTF8Util::TruncateUTF8(text, 16));
        EXPECT_EQ(text, UTF8Util::TruncateUTF8(text, length));
        EXPECT_EQ(text, UTF8Util::TruncateUTF8(text, length + 1));
    }

    TEST_F(UTF8UtilTest, GetByteMap) {
        std::vector<size_t> byteMap;
        UTF8Util::GetByteMap(text, 6, &byteMap);
        EXPECT_EQ(std::vector<size_t>({0, 3, 6, 9, 12, 16}), byteMap);
    }

} // namespace libnlp
