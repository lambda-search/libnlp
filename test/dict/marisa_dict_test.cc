
#include "libnlp/dict/marisa_dict.h"
#include "test_utils_utf8.h"
#include "text_dict_test_base.h"

namespace libnlp::dict {

    class MarisaDictTest : public TextDictTestBase {
    protected:
        MarisaDictTest()
                : dict(marisa_dict::create_from_dict(*textDict)), fileName("dict.ocd2") {};

        const marisa_dict_ptr dict;
        const std::string fileName;
    };

    TEST_F(MarisaDictTest, DictTest) { TestDict(dict); }

    TEST_F(MarisaDictTest, Serialization) {
        dict->libnlp::dict::serializable_dict::serialize_to_file(fileName);
    }

    TEST_F(MarisaDictTest, Deserialization) {
        const marisa_dict_ptr &deserialized =
                serializable_dict::create_from_file<marisa_dict>(fileName);
        TestDict(deserialized);

        const lexicon_ptr &lex1 = dict->get_lexicon();
        const lexicon_ptr &lex2 = deserialized->get_lexicon();

        // Compare every entry
        EXPECT_EQ(lex1->length(), lex2->length());
        for (size_t i = 0; i < lex1->length(); i++) {
            EXPECT_EQ(lex1->at(i)->key(), lex2->at(i)->key());
            EXPECT_EQ(lex1->at(i)->num_values(), lex2->at(i)->num_values());
        }
    }

    TEST_F(MarisaDictTest, ExactMatch) {
        auto there = dict->match("積羽沉舟", 12);
        EXPECT_FALSE(!there);
        auto dictEntry = *there;
        EXPECT_EQ(1, dictEntry->num_values());
        EXPECT_EQ(utf8("羣輕折軸"), dictEntry->get_default());

        auto nowhere = dict->match("積羽沉舟衆口鑠金", 24);
        EXPECT_TRUE(!nowhere);
    }

    TEST_F(MarisaDictTest, match_prefix) {
        {
            auto there = dict->match_prefix("清華", 3);
            EXPECT_FALSE(!there);
            auto dictEntry = *there;
            EXPECT_EQ(utf8("Tsing"), dictEntry->get_default());
        }
        {
            auto there = dict->match_prefix("清華", 5);
            EXPECT_FALSE(!there);
            auto dictEntry = *there;
            EXPECT_EQ(utf8("Tsing"), dictEntry->get_default());
        }
        {
            auto there = dict->match_prefix("清華", 6);
            EXPECT_FALSE(!there);
            auto dictEntry = *there;
            EXPECT_EQ(utf8("Tsinghua"), dictEntry->get_default());
        }
        {
            auto there = dict->match_prefix("清華", 100);
            EXPECT_FALSE(!there);
            auto dictEntry = *there;
            EXPECT_EQ(utf8("Tsinghua"), dictEntry->get_default());
        }
    }

} // namespace libnlp::cc
