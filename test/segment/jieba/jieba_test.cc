
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/segment/jieba/jieba.h"
#include "gtest/gtest.h"

using namespace libnlp::jieba;

TEST(JiebaTest, Test1) {
    libnlp::jieba::jieba_engine jieba(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8",
            JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
                          JIEBA_DICT_DATA_ROOT"/user.dict.utf8",
                          JIEBA_DICT_DATA_ROOT"/idf.utf8",
                          JIEBA_DICT_DATA_ROOT"/stop_words.utf8");
    vector<string> words;
    string result;

    jieba.cut("他来到了网易杭研大厦", words);
    result << words;
    ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);

    jieba.cut("我来自北京邮电大学。", words, false);
    result << words;
    ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", result);

    jieba.cut_small("南京市长江大桥", words, 3);
    ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", result << words);

    jieba.cut_hmm("我来自北京邮电大学。。。学号123456", words);
    result << words;
    ASSERT_EQ("[\"我来\", \"自北京\", \"邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\"]", result);

    jieba.cut("我来自北京邮电大学。。。学号123456，用AK47", words);
    result << words;
    ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\", \"，\", \"用\", \"AK47\"]", result);

    jieba.cut_all("我来自北京邮电大学", words);
    result << words;
    ASSERT_EQ(result, "[\"我\", \"来自\", \"北京\", \"北京邮电\", \"北京邮电大学\", \"邮电\", \"邮电大学\", \"电大\", \"大学\"]");

    jieba.cut_for_search("他来到了网易杭研大厦", words);
    result << words;
    ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);

}

TEST(JiebaTest, WordTest) {
    libnlp::jieba::jieba_engine jieba(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8",
                          JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
                          JIEBA_DICT_DATA_ROOT"/user.dict.utf8",
                          JIEBA_DICT_DATA_ROOT"/idf.utf8",
                          JIEBA_DICT_DATA_ROOT"/stop_words.utf8");
    vector<word_type> words;
    string result;

    jieba.cut("他来到了网易杭研大厦", words);
    result << words;
    ASSERT_EQ(
            "[{\"word\": \"\xE4\xBB\x96\", \"offset\": 0}, {\"word\": \"\xE6\x9D\xA5\xE5\x88\xB0\", \"offset\": 3}, {\"word\": \"\xE4\xBA\x86\", \"offset\": 9}, {\"word\": \"\xE7\xBD\x91\xE6\x98\x93\", \"offset\": 12}, {\"word\": \"\xE6\x9D\xAD\xE7\xA0\x94\", \"offset\": 18}, {\"word\": \"\xE5\xA4\xA7\xE5\x8E\xA6\", \"offset\": 24}]",
            result);

    jieba.cut("我来自北京邮电大学。", words, false);
    result << words;
    //ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\"]", result);
    ASSERT_EQ(
            "[{\"word\": \"\xE6\x88\x91\", \"offset\": 0}, {\"word\": \"\xE6\x9D\xA5\xE8\x87\xAA\", \"offset\": 3}, {\"word\": \"\xE5\x8C\x97\xE4\xBA\xAC\xE9\x82\xAE\xE7\x94\xB5\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 9}, {\"word\": \"\xE3\x80\x82\", \"offset\": 27}]",
            result);

    jieba.cut_small("南京市长江大桥", words, 3);
    //ASSERT_EQ("[\"南京市\", \"长江\", \"大桥\"]", result << words);
    ASSERT_EQ(
            "[{\"word\": \"\xE5\x8D\x97\xE4\xBA\xAC\xE5\xB8\x82\", \"offset\": 0}, {\"word\": \"\xE9\x95\xBF\xE6\xB1\x9F\", \"offset\": 9}, {\"word\": \"\xE5\xA4\xA7\xE6\xA1\xA5\", \"offset\": 15}]",
            result << words);

    jieba.cut_hmm("我来自北京邮电大学。。。学号123456", words);
    result << words;
    ASSERT_EQ(
            "[{\"word\": \"\xE6\x88\x91\xE6\x9D\xA5\", \"offset\": 0}, {\"word\": \"\xE8\x87\xAA\xE5\x8C\x97\xE4\xBA\xAC\", \"offset\": 6}, {\"word\": \"\xE9\x82\xAE\xE7\x94\xB5\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 15}, {\"word\": \"\xE3\x80\x82\", \"offset\": 27}, {\"word\": \"\xE3\x80\x82\", \"offset\": 30}, {\"word\": \"\xE3\x80\x82\", \"offset\": 33}, {\"word\": \"\xE5\xAD\xA6\xE5\x8F\xB7\", \"offset\": 36}, {\"word\": \"123456\", \"offset\": 42}]",
            result);

    jieba.cut("我来自北京邮电大学。。。学号123456，用AK47", words);
    result << words;
    //ASSERT_EQ("[\"我\", \"来自\", \"北京邮电大学\", \"。\", \"。\", \"。\", \"学号\", \"123456\", \"，\", \"用\", \"AK47\"]", result);
    ASSERT_EQ(
            "[{\"word\": \"\xE6\x88\x91\", \"offset\": 0}, {\"word\": \"\xE6\x9D\xA5\xE8\x87\xAA\", \"offset\": 3}, {\"word\": \"\xE5\x8C\x97\xE4\xBA\xAC\xE9\x82\xAE\xE7\x94\xB5\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 9}, {\"word\": \"\xE3\x80\x82\", \"offset\": 27}, {\"word\": \"\xE3\x80\x82\", \"offset\": 30}, {\"word\": \"\xE3\x80\x82\", \"offset\": 33}, {\"word\": \"\xE5\xAD\xA6\xE5\x8F\xB7\", \"offset\": 36}, {\"word\": \"123456\", \"offset\": 42}, {\"word\": \"\xEF\xBC\x8C\", \"offset\": 48}, {\"word\": \"\xE7\x94\xA8\", \"offset\": 51}, {\"word\": \"AK47\", \"offset\": 54}]",
            result);

    jieba.cut_all("我来自北京邮电大学", words);
    result << words;
    //ASSERT_EQ(result, "[\"我\", \"来自\", \"北京\", \"北京邮电\", \"北京邮电大学\", \"邮电\", \"邮电大学\", \"电大\", \"大学\"]");
    ASSERT_EQ(
            "[{\"word\": \"\xE6\x88\x91\", \"offset\": 0}, {\"word\": \"\xE6\x9D\xA5\xE8\x87\xAA\", \"offset\": 3}, {\"word\": \"\xE5\x8C\x97\xE4\xBA\xAC\", \"offset\": 9}, {\"word\": \"\xE5\x8C\x97\xE4\xBA\xAC\xE9\x82\xAE\xE7\x94\xB5\", \"offset\": 9}, {\"word\": \"\xE5\x8C\x97\xE4\xBA\xAC\xE9\x82\xAE\xE7\x94\xB5\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 9}, {\"word\": \"\xE9\x82\xAE\xE7\x94\xB5\", \"offset\": 15}, {\"word\": \"\xE9\x82\xAE\xE7\x94\xB5\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 15}, {\"word\": \"\xE7\x94\xB5\xE5\xA4\xA7\", \"offset\": 18}, {\"word\": \"\xE5\xA4\xA7\xE5\xAD\xA6\", \"offset\": 21}]",
            result);

    jieba.cut_for_search("他来到了网易杭研大厦", words);
    result << words;
    //ASSERT_EQ("[\"他\", \"来到\", \"了\", \"网易\", \"杭研\", \"大厦\"]", result);
    ASSERT_EQ(
            "[{\"word\": \"\xE4\xBB\x96\", \"offset\": 0}, {\"word\": \"\xE6\x9D\xA5\xE5\x88\xB0\", \"offset\": 3}, {\"word\": \"\xE4\xBA\x86\", \"offset\": 9}, {\"word\": \"\xE7\xBD\x91\xE6\x98\x93\", \"offset\": 12}, {\"word\": \"\xE6\x9D\xAD\xE7\xA0\x94\", \"offset\": 18}, {\"word\": \"\xE5\xA4\xA7\xE5\x8E\xA6\", \"offset\": 24}]",
            result);
}

TEST(JiebaTest, insert_user_word) {
    libnlp::jieba::jieba_engine jieba(JIEBA_DICT_DATA_ROOT"/jieba.dict.utf8",
                          JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
                          JIEBA_DICT_DATA_ROOT"/user.dict.utf8",
                          JIEBA_DICT_DATA_ROOT"/idf.utf8",
                          JIEBA_DICT_DATA_ROOT"/stop_words.utf8");
    vector<string> words;
    string result;

    jieba.cut("男默女泪", words);
    result << words;
    ASSERT_EQ("[\"男默\", \"女泪\"]", result);

    ASSERT_TRUE(jieba.insert_user_word("男默女泪"));

    jieba.cut("男默女泪", words);
    result << words;
    ASSERT_EQ("[\"男默女泪\"]", result);

    for (size_t i = 0; i < 100; i++) {
        string newWord;
        newWord << rand();
        ASSERT_TRUE(jieba.insert_user_word(newWord));
        jieba.cut(newWord, words);
        result << words;
        ASSERT_EQ(result, StringFormat("[\"%s\"]", newWord.c_str()));
    }

    ASSERT_TRUE(jieba.insert_user_word("同一个世界，同一个梦想"));
    jieba.cut("同一个世界，同一个梦想", words);
    result = Join(words.begin(), words.end(), "/");
    ASSERT_EQ(result, "同一个/世界/，/同一个/梦想");

    jieba.reset_separators("");

    jieba.cut("同一个世界，同一个梦想", words);
    result = Join(words.begin(), words.end(), "/");
    ASSERT_EQ(result, "同一个世界，同一个梦想");

    {
        string s("一部iPhone6");
        string res;
        vector<keyword_extractor::word_type> wordweights;
        size_t topN = 5;
        jieba.extractor.extract(s, wordweights, topN);
        res << wordweights;
        ASSERT_EQ(res,
                  "[{\"word\": \"iPhone6\", \"offset\": [6], \"weight\": 11.7392}, {\"word\": \"\xE4\xB8\x80\xE9\x83\xA8\", \"offset\": [0], \"weight\": 6.47592}]");
    }
}
