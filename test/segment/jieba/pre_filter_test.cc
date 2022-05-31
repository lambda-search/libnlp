
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "gtest/gtest.h"
#include "libnlp/segment/jieba/pre_filter.h"
#include "libnlp/base/string_util.h"
#include <vector>
#include <string>
#include <unordered_set>

using std::vector;
using std::string;
using std::pair;

using namespace libnlp::jieba;
using namespace libnlp;

TEST(PreFilterTest, Test1) {
    std::unordered_set <libnlp::rune_t> symbol;
    symbol.insert(65292u); // "，"
    symbol.insert(12290u); // "。"
    string expected;
    string res;

    {
        string s = "你好，美丽的，世界";
        pre_filter filter(symbol, s);
        expected = "你好/，/美丽的/，/世界";
        ASSERT_TRUE(filter.has_next());
        vector<string> words;
        while (filter.has_next()) {
            pre_filter::range range;
            range = filter.next();
            words.push_back(get_string_from_runes(s, range.begin, range.end - 1));
        }
        res = libnlp::Join(words.begin(), words.end(), "/");
        ASSERT_EQ(res, expected);
    }

    {
        string s = "我来自北京邮电大学。。。学号123456，用AK47";
        pre_filter filter(symbol, s);
        expected = "我来自北京邮电大学/。/。/。/学号123456/，/用AK47";
        ASSERT_TRUE(filter.has_next());
        vector<string> words;
        while (filter.has_next()) {
            pre_filter::range range;
            range = filter.next();
            words.push_back(get_string_from_runes(s, range.begin, range.end - 1));
        }
        res = libnlp::Join(words.begin(), words.end(), "/");
        ASSERT_EQ(res, expected);
    }
}
