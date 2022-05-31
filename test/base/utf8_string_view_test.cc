
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/



#include "libnlp/base/utf8_string_view.h"
#include "test_utils.h"

namespace libnlp {

    class UTF8StringSliceTest : public ::testing::Test {
    protected:
        UTF8StringSliceTest()
                : text("天行健，君子以自強不息。地勢坤，君子以厚德載物。"), empty("") {};

        UTF8StringSlice text;
        const UTF8StringSlice empty;
    };

    TEST_F(UTF8StringSliceTest, UTF8Length) {
        EXPECT_EQ(0, empty.UTF8Length());
        EXPECT_EQ(24, text.UTF8Length());
    }

    TEST_F(UTF8StringSliceTest, ByteLength) {
        EXPECT_EQ(0, empty.ByteLength());
        EXPECT_EQ(72, text.ByteLength());
    }

    TEST_F(UTF8StringSliceTest, Left) {
        EXPECT_EQ(UTF8StringSlice("天行健"), text.Left(3));
    }

    TEST_F(UTF8StringSliceTest, Right) {
        EXPECT_EQ(UTF8StringSlice("厚德載物。"), text.Right(5));
    }

    TEST_F(UTF8StringSliceTest, SubString) {
        EXPECT_EQ(UTF8StringSlice("自強不息"), text.SubString(7, 4));
    }

    TEST_F(UTF8StringSliceTest, to_string) {
        EXPECT_EQ("地勢坤", text.SubString(12, 3).to_string());
    }

    TEST_F(UTF8StringSliceTest, Compare) {
        EXPECT_TRUE(text.SubString(12, 3) > UTF8StringSlice("一"));
        EXPECT_TRUE(text.SubString(12, 3) == UTF8StringSlice("地勢坤"));
    }

    TEST_F(UTF8StringSliceTest, MoveRight) {
        text.MoveRight();
        EXPECT_EQ(UTF8StringSlice("行健，君子以自強不息。地勢坤，君子以厚德載物。"),
                  text);
        for (size_t i = 0; i < 23; i++) {
            text.MoveRight();
        }
        EXPECT_EQ(UTF8StringSlice(""), text);
        text.MoveRight(); // No effect, because it's already empty
        EXPECT_EQ(UTF8StringSlice(""), text);
    }

    TEST_F(UTF8StringSliceTest, MoveLeft) {
        text.MoveLeft();
        EXPECT_EQ(UTF8StringSlice("天行健，君子以自強不息。地勢坤，君子以厚德載物"),
                  text);
        for (size_t i = 0; i < 22; i++) {
            text.MoveLeft();
        }
        EXPECT_EQ(UTF8StringSlice("天"), text);
        text.MoveLeft();
        text.MoveLeft(); // No effect, because it's already empty
        EXPECT_EQ(UTF8StringSlice(""), text);
    }

    TEST_F(UTF8StringSliceTest, ReverseCompare) {
        EXPECT_EQ(0, UTF8StringSlice("").ReverseCompare(UTF8StringSlice("")));
        EXPECT_EQ(-1, UTF8StringSlice("").ReverseCompare(UTF8StringSlice("大")));
        EXPECT_EQ(-1, UTF8StringSlice("一").ReverseCompare(UTF8StringSlice("二")));
        EXPECT_EQ(-1, UTF8StringSlice("z一").ReverseCompare(UTF8StringSlice("a二")));
        EXPECT_EQ(1, UTF8StringSlice("一一").ReverseCompare(UTF8StringSlice("一")));
    }

    TEST_F(UTF8StringSliceTest, FindBytePosition) {
        EXPECT_EQ(0, text.FindBytePosition(""));
        EXPECT_EQ(0, text.FindBytePosition("天"));
        EXPECT_EQ(9, text.FindBytePosition("，"));
        EXPECT_EQ(static_cast<UTF8StringSlice::LengthType>(-1),
                  text.FindBytePosition("a"));
        EXPECT_EQ(static_cast<UTF8StringSlice::LengthType>(-1),
                  text.FindBytePosition("\n"));
        EXPECT_EQ(3, UTF8StringSlice("了。").FindBytePosition("。"));
    }

} // namespace libnlp
