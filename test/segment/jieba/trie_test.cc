
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#include "libnlp/segment/jieba/dict_trie.h"
#include "libnlp/segment/jieba/mp_segment.h"
#include "gtest/gtest.h"

using namespace libnlp::jieba;

static const char *const DICT_FILE = JIEBA_TEST_DATA_ROOT
"/extra_dict/jieba.dict.small.utf8";

TEST(TrieTest, Empty) {
    vector<unicode> keys;
    vector<const dict_unit *> values;
    base_trie trie(keys, values);
}

TEST(TrieTest, Construct) {
    vector<unicode> keys;
    vector<const dict_unit *> values;
    keys.push_back(decode_runes_in_string("你"));
    values.push_back((const dict_unit *) (nullptr));
    base_trie trie(keys, values);
}

TEST(DictTrieTest, NewAndDelete) {
    dict_trie *trie;
    trie = new dict_trie(DICT_FILE);
    delete trie;
}

TEST(DictTrieTest, Test1) {
    string s1, s2;
    dict_trie trie(DICT_FILE);
    ASSERT_LT(trie.get_min_weight() + 15.6479, 0.001);
    string word("来到");
    libnlp::jieba::rune_str_array uni;
    ASSERT_TRUE(decode_runes_in_string(word, uni));
    //dict_unit nodeInfo;
    //nodeInfo.word = uni;
    //nodeInfo.tag = "v";
    //nodeInfo.weight = -8.87033;
    //s1 << nodeInfo;
    //s2 << (*trie.find(uni.begin(), uni.end()));
    const dict_unit *du = trie.find(uni.begin(), uni.end());
    ASSERT_TRUE(du != nullptr);
    ASSERT_EQ(2u, du->word.size());
    ASSERT_EQ(26469u, du->word[0]);
    ASSERT_EQ(21040u, du->word[1]);
    ASSERT_EQ("v", du->tag);
    ASSERT_NEAR(-8.870, du->weight, 0.001);

    //EXPECT_EQ("[\"26469\", \"21040\"] v -8.870", s2);
    word = "清华大学";
    libnlp::inline_vector <std::pair<size_t, const dict_unit *>> res;
    const char *words[] = {"清", "清华", "清华大学"};
    for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        ASSERT_TRUE(decode_runes_in_string(words[i], uni));
        res.push_back(std::make_pair(uni.size() - 1, trie.find(uni.begin(), uni.end())));
        //resMap[uni.size() - 1] = trie.find(uni.begin(), uni.end());
    }
    std::vector<std::pair<size_t, const dict_unit *> > vec;
    std::vector<struct dag_entity> dags;
    ASSERT_TRUE(decode_runes_in_string(word, uni));
    trie.find(uni.begin(), uni.end(), dags);
    ASSERT_EQ(dags.size(), uni.size());
    ASSERT_NE(dags.size(), 0u);
    s1 << res;
    s2 << dags[0].nexts;
    ASSERT_EQ(s1, s2);

}

TEST(DictTrieTest, UserDict) {
    dict_trie trie(DICT_FILE, JIEBA_TEST_DATA_ROOT"/userdict.utf8");
    string word = "云计算";
    libnlp::jieba::rune_str_array unicode;
    ASSERT_TRUE(decode_runes_in_string(word, unicode));
    const dict_unit *unit = trie.find(unicode.begin(), unicode.end());
    ASSERT_TRUE(unit != nullptr);
    ASSERT_NEAR(unit->weight, -14.100, 0.001);

    word = "蓝翔";
    ASSERT_TRUE(decode_runes_in_string(word, unicode));
    unit = trie.find(unicode.begin(), unicode.end());
    ASSERT_TRUE(unit != nullptr);
    ASSERT_EQ(unit->tag, "nz");
    ASSERT_NEAR(unit->weight, -14.100, 0.001);

    word = "区块链";
    ASSERT_TRUE(decode_runes_in_string(word, unicode));
    unit = trie.find(unicode.begin(), unicode.end());
    ASSERT_TRUE(unit != nullptr);
    ASSERT_EQ(unit->tag, "nz");
    ASSERT_NEAR(unit->weight, -15.6478, 0.001);
}

TEST(DictTrieTest, UserDictWithMaxWeight) {
    dict_trie trie(DICT_FILE, JIEBA_TEST_DATA_ROOT"/userdict.utf8", dict_trie::WordWeightMax);
    string word = "云计算";
    libnlp::jieba::rune_str_array unicode;
    ASSERT_TRUE(decode_runes_in_string(word, unicode));
    const dict_unit *unit = trie.find(unicode.begin(), unicode.end());
    ASSERT_TRUE(unit);
    ASSERT_NEAR(unit->weight, -2.975, 0.001);
}

TEST(DictTrieTest, dag_entity) {
    dict_trie trie(DICT_FILE, JIEBA_TEST_DATA_ROOT"/userdict.utf8");

    {
        string word = "清华大学";
        libnlp::jieba::rune_str_array unicode;
        ASSERT_TRUE(decode_runes_in_string(word, unicode));
        vector<struct dag_entity> res;
        trie.find(unicode.begin(), unicode.end(), res);

        size_t nexts_sizes[] = {3, 2, 2, 1};
        ASSERT_EQ(res.size(), sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));
        for (size_t i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
        }
    }

    {
        string word = "北京邮电大学";
        libnlp::jieba::rune_str_array unicode;
        ASSERT_TRUE(decode_runes_in_string(word, unicode));
        vector<struct dag_entity> res;
        trie.find(unicode.begin(), unicode.end(), res);

        size_t nexts_sizes[] = {3, 1, 2, 2, 2, 1};
        ASSERT_EQ(res.size(), sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));
        for (size_t i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
        }
    }

    {
        string word = "长江大桥";
        libnlp::jieba::rune_str_array unicode;
        ASSERT_TRUE(decode_runes_in_string(word, unicode));
        vector<struct dag_entity> res;
        trie.find(unicode.begin(), unicode.end(), res);

        size_t nexts_sizes[] = {3, 1, 2, 1};
        ASSERT_EQ(res.size(), sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));
        for (size_t i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
        }
    }

    {
        string word = "长江大桥";
        libnlp::jieba::rune_str_array unicode;
        ASSERT_TRUE(decode_runes_in_string(word, unicode));
        vector<struct dag_entity> res;
        trie.find(unicode.begin(), unicode.end(), res, 3);

        size_t nexts_sizes[] = {2, 1, 2, 1};
        ASSERT_EQ(res.size(), sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));
        for (size_t i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
        }
    }

    {
        string word = "长江大桥";
        libnlp::jieba::rune_str_array unicode;
        ASSERT_TRUE(decode_runes_in_string(word, unicode));
        vector<struct dag_entity> res;
        trie.find(unicode.begin(), unicode.end(), res, 4);

        size_t nexts_sizes[] = {3, 1, 2, 1};
        ASSERT_EQ(res.size(), sizeof(nexts_sizes) / sizeof(nexts_sizes[0]));
        for (size_t i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i].nexts.size(), nexts_sizes[i]);
        }
    }
}
