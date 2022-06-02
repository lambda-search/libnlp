
#pragma once

#include "libnlp/dict/lexicon.h"
#include "test_utils.h"
#include "test_utils_utf8.h"
#include "libnlp/dict/text_dict.h"

namespace libnlp::dict {

    class TextDictTestBase : public ::testing::Test {
    protected:
        TextDictTestBase() : textDict(CreateTextDictForText()) {};

        text_dict_ptr CreateTextDictForText() const {
            lexicon_ptr lex(new lexicon);
            lex->add(dict_entity::create("BYVoid", "byv"));
            lex->add(dict_entity::create("zigzagzig", "zag"));
            lex->add(dict_entity::create(utf8("積羽沉舟"), utf8("羣輕折軸")));
            lex->add(dict_entity::create(utf8("清"), "Tsing"));
            lex->add(dict_entity::create(utf8("清華"), "Tsinghua"));
            lex->add(dict_entity::create(utf8("清華大學"), "TsinghuaUniversity"));
            lex->sort();
            return text_dict_ptr(new text_dict(lex));
        }

        basic_dict_ptr CreateDictForCharacters() const {
            lexicon_ptr lex(new lexicon);
            lex->add(dict_entity::create(
                    utf8("后"), std::vector<std::string>{utf8("后"), utf8("後")}));
            lex->add(dict_entity::create(
                    utf8("发"), std::vector<std::string>{utf8("發"), utf8("髮")}));
            lex->add(dict_entity::create(
                    utf8("干"),
                    std::vector<std::string>{utf8("幹"), utf8("乾"), utf8("干")}));
            lex->add(dict_entity::create(
                    utf8("里"), std::vector<std::string>{utf8("裏"), utf8("里")}));
            lex->sort();
            return text_dict_ptr(new text_dict(lex));
        }

        basic_dict_ptr CreateDictForPhrases() const {
            lexicon_ptr lex(new lexicon);
            lex->add(dict_entity::create(utf8("太后"), utf8("太后")));
            lex->add(dict_entity::create(utf8("头发"), utf8("頭髮")));
            lex->add(dict_entity::create(utf8("干燥"), utf8("乾燥")));
            lex->add(dict_entity::create(utf8("鼠标"), utf8("鼠標")));
            lex->sort();
            return text_dict_ptr(new text_dict(lex));
        }

        basic_dict_ptr CreateDictForTaiwanVariants() const {
            lexicon_ptr lex(new lexicon);
            lex->add(dict_entity::create(utf8("裏"), utf8("裡")));
            return text_dict_ptr(new text_dict(lex));
        }

        basic_dict_ptr CreateTaiwanPhraseDict() const {
            lexicon_ptr lex(new lexicon);
            lex->add(dict_entity::create(utf8("鼠标"), utf8("滑鼠")));
            lex->add(dict_entity::create(utf8("服务器"), utf8("伺服器")));
            lex->add(dict_entity::create(utf8("克罗地亚"), utf8("克羅埃西亞")));
            lex->sort();
            return text_dict_ptr(new text_dict(lex));
        }

        void TestDict(const basic_dict_ptr dict) const {
            TestMatch(dict);
            TestMatchPrefix(dict);
            TestMatchAllPrefixes(dict);
        }

        void TestMatch(const basic_dict_ptr &dict) const {
            std::optional<const dict_entity *> entry;
            entry = dict->match("BYVoid");
            EXPECT_TRUE(!!entry);
            EXPECT_EQ(utf8("BYVoid"), (*entry)->key());
            EXPECT_EQ(utf8("byv"), (*entry)->get_default());

            entry = dict->match("");
            EXPECT_TRUE(!entry);

            entry = dict->match("xxx");
            EXPECT_TRUE(!entry);
        }

        void TestMatchPrefix(const basic_dict_ptr &dict) const {
            std::optional<const dict_entity *> entry;
            entry = dict->match_prefix("BYVoid");
            EXPECT_TRUE(!!entry);
            EXPECT_EQ(utf8("BYVoid"), (*entry)->key());
            EXPECT_EQ(utf8("byv"), (*entry)->get_default());

            entry = dict->match_prefix(utf8("清華大學"));
            EXPECT_TRUE(!!entry);
            EXPECT_EQ(utf8("清華大學"), (*entry)->key());
            EXPECT_EQ(utf8("TsinghuaUniversity"), (*entry)->get_default());

            entry = dict->match_prefix("BYVoid123");
            EXPECT_TRUE(!!entry);
            EXPECT_EQ(utf8("BYVoid"), (*entry)->key());
            EXPECT_EQ(utf8("byv"), (*entry)->get_default());

            entry = dict->match_prefix(utf8("積羽沉舟"));
            EXPECT_TRUE(!!entry);
            EXPECT_EQ(utf8("積羽沉舟"), (*entry)->key());
            EXPECT_EQ(utf8("羣輕折軸"), (*entry)->get_default());

            entry = dict->match_prefix("Unknown");
            EXPECT_TRUE(!entry);

            entry = dict->match_prefix("");
            EXPECT_TRUE(!entry);
        }

        void TestMatchAllPrefixes(const basic_dict_ptr &dict) const {
            const std::vector<const dict_entity *> matches =
                    dict->match_all_prefixes(utf8("清華大學計算機系"));
            EXPECT_EQ(3, matches.size());
            EXPECT_EQ(utf8("清華大學"), matches.at(0)->key());
            EXPECT_EQ(utf8("TsinghuaUniversity"), matches.at(0)->get_default());
            EXPECT_EQ(utf8("清華"), matches.at(1)->key());
            EXPECT_EQ(utf8("Tsinghua"), matches.at(1)->get_default());
            EXPECT_EQ(utf8("清"), matches.at(2)->key());
            EXPECT_EQ(utf8("Tsing"), matches.at(2)->get_default());
        }

        const text_dict_ptr textDict;
    };

} // namespace libnlp::cc
