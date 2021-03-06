
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include "libnlp/segment/jieba/keyword_extractor.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

using namespace libnlp::jieba;

TEST(KeywordExtractorTest, Test1) {
    keyword_extractor
    Extractor(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8",
              JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
              JIEBA_DICT_DATA_ROOT"/idf.utf8",
              JIEBA_DICT_DATA_ROOT"/stop_words.utf8");
    {
        std::string s("你好世界世界而且而且");
        std::string res;
        size_t topN = 5;

        {
            std::vector <std::string> words;
            Extractor.extract(s, words, topN);
            res << words;
            ASSERT_EQ(res, "[\"世界\", \"你好\"]");
        }

        {
            std::vector <std::pair<std::string, double>> words;
            Extractor.extract(s, words, topN);
            res << words;
            ASSERT_EQ(res, "[世界:8.73506, 你好:7.95788]");
        }

        {
            std::vector <keyword_extractor::word_type> words;
            Extractor.extract(s, words, topN);
            res << words;
            ASSERT_EQ(res,
                      "[{\"word\": \"\xE4\xB8\x96\xE7\x95\x8C\", \"offset\": [6, 12], \"weight\": 8.73506}, {\"word\": \"\xE4\xBD\xA0\xE5\xA5\xBD\", \"offset\": [0], \"weight\": 7.95788}]");
        }
    }

    {
        std::string s("我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。");
        std::string res;
        std::vector <keyword_extractor::word_type> wordweights;
        size_t topN = 5;
        Extractor.extract(s, wordweights, topN);
        res << wordweights;
        ASSERT_EQ(res,
                  "[{\"word\": \"CEO\", \"offset\": [93], \"weight\": 11.7392}, {\"word\": \"\xE5\x8D\x87\xE8\x81\x8C\", \"offset\": [72], \"weight\": 10.8562}, {\"word\": \"\xE5\x8A\xA0\xE8\x96\xAA\", \"offset\": [78], \"weight\": 10.6426}, {\"word\": \"\xE6\x89\x8B\xE6\x89\xB6\xE6\x8B\x96\xE6\x8B\x89\xE6\x9C\xBA\", \"offset\": [21], \"weight\": 10.0089}, {\"word\": \"\xE5\xB7\x85\xE5\xB3\xB0\", \"offset\": [111], \"weight\": 9.49396}]");
    }

    {
        std::string s("一部iPhone6");
        std::string res;
        std::vector <keyword_extractor::word_type> wordweights;
        size_t topN = 5;
        Extractor.extract(s, wordweights, topN);
        res << wordweights;
        ASSERT_EQ(res,
                  "[{\"word\": \"iPhone6\", \"offset\": [6], \"weight\": 11.7392}, {\"word\": \"\xE4\xB8\x80\xE9\x83\xA8\", \"offset\": [0], \"weight\": 6.47592}]");
    }
}

TEST(KeywordExtractorTest, Test2) {
    keyword_extractor Extractor(JIEBA_TEST_DATA_ROOT"/extra_dict/jieba.dict.small.utf8", JIEBA_DICT_DATA_ROOT"/hmm_model.utf8",
            JIEBA_DICT_DATA_ROOT"/idf.utf8", JIEBA_DICT_DATA_ROOT"/stop_words.utf8", JIEBA_TEST_DATA_ROOT"/userdict.utf8");

    {
        string s("蓝翔优秀毕业生");
        string res;
        vector <keyword_extractor::word_type> wordweights;
        size_t topN = 5;
        Extractor.extract(s, wordweights, topN);
        res << wordweights;
        ASSERT_EQ(res,
                  "[{\"word\": \"\xE8\x93\x9D\xE7\xBF\x94\", \"offset\": [0], \"weight\": 11.7392}, {\"word\": \"\xE6\xAF\x95\xE4\xB8\x9A\xE7\x94\x9F\", \"offset\": [12], \"weight\": 8.13549}, {\"word\": \"\xE4\xBC\x98\xE7\xA7\x80\", \"offset\": [6], \"weight\": 6.78347}]");
    }

    {
        string s("一部iPhone6");
        string res;
        vector <keyword_extractor::word_type> wordweights;
        size_t topN = 5;
        Extractor.extract(s, wordweights, topN);
        res << wordweights;
        ASSERT_EQ(res,
                  "[{\"word\": \"iPhone6\", \"offset\": [6], \"weight\": 11.7392}, {\"word\": \"\xE4\xB8\x80\xE9\x83\xA8\", \"offset\": [0], \"weight\": 6.47592}]");
    }
}
