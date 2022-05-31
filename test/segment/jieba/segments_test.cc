

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/segment/jieba/segment_base.h"
#include "libnlp/segment/jieba/mix_segment.h"
#include "libnlp/segment/jieba/mp_segment.h"
#include "libnlp/segment/jieba/hmm_model.h"
#include "libnlp/segment/jieba/hmm_segment.h"
#include "libnlp/segment/jieba/full_segment.h"
#include "libnlp/segment/jieba/query_segment.h"
#include "gtest/gtest.h"
#include <string>

using namespace libnlp::jieba;

TEST(MixSegmentTest, Test1) {
    mix_segment
    segment(JIEBA_DICT_DATA_ROOT
    "/jieba.dict.utf8", JIEBA_DICT_DATA_ROOT
    "/hmm_model.utf8");;
    string sentence;
    vector<string> words;
    string actual;
    string expected;

    {
        sentence = "我来自北京邮电大学。。。学号123456，用AK47";
        expected = "我/来自/北京邮电大学/。/。/。/学号/123456/，/用/AK47";
        segment.cut(sentence, words);
        actual = Join(words.begin(), words.end(), "/");
        ASSERT_EQ(actual, expected);
    }

    {
        sentence = "B超 T恤";
        expected = "B超/ /T恤";
        segment.cut(sentence, words);
        actual = Join(words.begin(), words.end(), "/");
        ASSERT_EQ(actual, expected);
    }

    {
        sentence = "他来到了网易杭研大厦";
        expected = "他/来到/了/网易/杭/研/大厦";
        segment.cut(sentence, words, false);
        actual = Join(words.begin(), words.end(), "/");
        ASSERT_EQ(actual, expected);
    }

    {
        sentence = "他来到了网易杭研大厦";
        expected = "他/来到/了/网易/杭研/大厦";
        segment.cut(sentence, words);
        actual = Join(words.begin(), words.end(), "/");
        ASSERT_EQ(actual, expected);
    }
}

TEST(MixSegmentTest, NoUserDict) {
    mix_segment
    segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8", JIEBA_DICT_DATA_ROOT
    "/hmm_model.utf8");
    const char *str = "令狐冲是云计算方面的专家";
    vector<string> words;
    segment.cut(str, words);
    string res;
    ASSERT_EQ("[\"令狐冲\", \"是\", \"云\", \"计算\", \"方面\", \"的\", \"专家\"]", res << words);

}

TEST(MixSegmentTest, UserDict) {
    mix_segment
    segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8", JIEBA_DICT_DATA_ROOT
    "/hmm_model.utf8", JIEBA_DICT_DATA_ROOT
    "/user.dict.utf8");
    {
        const char *str = "令狐冲是云计算方面的专家";
        vector<string> words;
        segment.cut(str, words);
        string res;
        ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);
    }
    {
        const char *str = "小明先就职于IBM,后在日本京都大学深造";
        vector<string> words;
        segment.cut(str, words);
        string res;
        res << words;
        ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"IBM\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]", res);
    }
    {
        const char *str = "IBM,3.14";
        vector<string> words;
        segment.cut(str, words);
        string res;
        res << words;
        ASSERT_EQ("[\"IBM\", \",\", \"3.14\"]", res);
    }
}

TEST(MixSegmentTest, TestUserDict) {
    mix_segment
    segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8", JIEBA_DICT_DATA_ROOT
    "/hmm_model.utf8",
            JIEBA_TEST_DATA_ROOT"/userdict.utf8");
    vector<string> words;
    string res;

    segment.cut("令狐冲是云计算方面的专家", words);
    ASSERT_EQ("[\"令狐冲\", \"是\", \"云计算\", \"方面\", \"的\", \"专家\"]", res << words);

    segment.cut("小明先就职于IBM,后在日本京都大学深造", words);
    res << words;
    ASSERT_EQ("[\"小明\", \"先\", \"就职\", \"于\", \"I\", \"B\", \"M\", \",\", \"后\", \"在\", \"日本\", \"京都大学\", \"深造\"]",
              res);

    segment.cut("IBM,3.14", words);
    res << words;
    ASSERT_EQ("[\"I\", \"B\", \"M\", \",\", \"3.14\"]", res);

    segment.cut("忽如一夜春风来，千树万树梨花开", words);
    res = libnlp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ("忽如一夜春风来/，/千树/万树/梨花/开", res);

    // rand input
    {
        const size_t ITERATION = 16;
        const size_t MAX_LEN = 256;
        string s;
        srand(time(nullptr));

        for (size_t i = 0; i < ITERATION; i++) {
            size_t len = rand() % MAX_LEN;
            s.resize(len);
            for (size_t j = 0; j < len; j++) {
                s[rand() % len] = rand();
            }
            segment.cut(s, words);
        }
    }
}

TEST(MixSegmentTest, TestMultiUserDict) {
    std::string dicts;
    dicts = JIEBA_TEST_DATA_ROOT;
    dicts += "/userdict.utf8;";
    dicts += JIEBA_TEST_DATA_ROOT;
    dicts += "/userdict.2.utf8";

    mix_segment segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8", JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
            dicts);
    vector<string> words;
    string res;

    segment.cut("忽如一夜春风来，千树万树梨花开", words);
    res = libnlp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ("忽如一夜春风来/，/千树万树梨花开", res);
}

TEST(MPSegmentTest, Test1) {
    mp_segment
    segment(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8");;
    string s;
    vector<string> words;
    segment.cut("我来自北京邮电大学。", words);
    ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", s << words);

    segment.cut("B超 T恤", words);
    ASSERT_EQ(s << words, "[\"B超\", \" \", \"T恤\"]");

    segment.cut("南京市长江大桥", words);
    ASSERT_EQ("[\"南京市\", \"长江大桥\"]", s << words);

    // MaxWordLen
    segment.cut("南京市长江大桥", words, 3);
    ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", s << words);

    segment.cut("南京市长江大桥", words, 0);
    ASSERT_EQ("[\"南\", \"京\", \"市\", \"长\", \"江\", \"大\", \"桥\"]", s << words);

    segment.cut("湖南长沙市天心区", words);
    s = Join(words.begin(), words.end(), "/");
    ASSERT_EQ("湖南长沙市/天心区", s);

    segment.cut("湖南长沙市天心区", words, 3);
    s = Join(words.begin(), words.end(), "/");
    ASSERT_EQ("湖南/长沙市/天心区", s);
}

TEST(HMMSegmentTest, Test1) {
    hmm_segment segment(JIEBA_DICT_DATA_ROOT"/hmm_model.utf8");;
    {
        const char *str = "我来自北京邮电大学。。。学号123456";
        const char *res[] = {"我来", "自北京", "邮电大学", "。", "。", "。", "学号", "123456"};
        vector<string> words;
        segment.cut(str, words);
        ASSERT_EQ(words, vector<string>(res, res + sizeof(res) / sizeof(res[0])));
    }

    {
        const char *str = "IBM,1.2,123";
        const char *res[] = {"IBM", ",", "1.2", ",", "123"};
        vector<string> words;
        segment.cut(str, words);
        ASSERT_EQ(words, vector<string>(res, res + sizeof(res) / sizeof(res[0])));
    }
}

TEST(full_segment, Test1) {
    full_segment segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8");
    vector<string> words;
    string s;

    segment.cut("我来自北京邮电大学", words);
    s << words;
    ASSERT_EQ(s, "[\"我\", \"来自\", \"北京\", \"北京邮电大学\", \"邮电\", \"电大\", \"大学\"]");


    segment.cut("上市公司CEO", words);
    s << words;
    ASSERT_EQ(s, "[\"上市\", \"公司\", \"C\", \"E\", \"O\"]");
}

TEST(query_segment, Test1) {
    query_segment
    segment(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8",
            JIEBA_DICT_DATA_ROOT"/hmm_model.utf8", "");
    vector<string> words;
    string s1, s2;

    segment.cut("小明硕士毕业于中国科学院计算所，后在日本京都大学深造", words);
    s1 = Join(words.begin(), words.end(), "/");
    s2 = "小明/硕士/毕业/于/中国/科学/学院/科学院/中国科学院/计算/计算所/，/后/在/日本/京都/大学/日本京都大学/深造";
    ASSERT_EQ(s1, s2);

    segment.cut("亲口交代", words);
    s1 = Join(words.begin(), words.end(), "/");
    s2 = "亲口/交代";
    ASSERT_EQ(s1, s2);

    segment.cut("他心理健康", words);
    s1 = Join(words.begin(), words.end(), "/");
    s2 = "他/心理/健康/心理健康";
    ASSERT_EQ(s1, s2);
}

TEST(query_segment, Test2) {
    std::string dicts =  JIEBA_TEST_DATA_ROOT"/userdict.utf8|";
    dicts += JIEBA_TEST_DATA_ROOT"/userdict.english";
    query_segment
    segment(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8",
            JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",dicts);
    vector<string> words;
    string s1, s2;

    {
        segment.cut("小明硕士毕业于中国科学院计算所，后在日本京都大学深造", words);
        s1 = Join(words.begin(), words.end(), "/");
        s2 = "小明/硕士/毕业/于/中国/科学/学院/科学院/中国科学院/计算/计算所/，/后/在/日本/京都/大学/京都大学/深造";
        ASSERT_EQ(s1, s2);
    }

    {
        segment.cut("小明硕士毕业于中国科学院计算所iPhone6", words);
        s1 = Join(words.begin(), words.end(), "/");
        s2 = "小明/硕士/毕业/于/中国/科学/学院/科学院/中国科学院/计算/计算所/iPhone6";
        ASSERT_EQ(s1, s2);
    }

    {
        segment.cut("中国科学院", words);
        s1 = Join(words.begin(), words.end(), "/");
        s2 = "中国/科学/学院/科学院/中国科学院";
        ASSERT_EQ(s1, s2);
    }

}

TEST(MPSegmentTest, Unicode32) {
    string s("天气很好，🙋 我们去郊游。");
    vector<string> words;

    mp_segment
    segment(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8");;
    segment.cut(s, words);

    ASSERT_EQ(Join(words.begin(), words.end(), "/"), "天气/很/好/，/🙋/ /我们/去/郊游/。");
}
