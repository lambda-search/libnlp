

#include "libnlp/dict/darts_dict.h"
#include "test_utils_utf8.h"
#include "text_dict_test_base.h"

namespace libnlp::dict {

    class DartsDictTest : public TextDictTestBase {
    protected:
        DartsDictTest()
                : dartsDict(darts_dict::create_from_dict(*textDict.get())),
                  fileName("dict.ocd") {};

        const darts_dict_ptr dartsDict;
        const std::string fileName;
    };

    TEST_F(DartsDictTest, DictTest) { TestDict(dartsDict); }

    TEST_F(DartsDictTest, Serialization) {
        dartsDict->libnlp::dict::serializable_dict::serialize_to_file(fileName);
    }

    TEST_F(DartsDictTest, Deserialization) {
        const darts_dict_ptr &deserialized =
                serializable_dict::create_from_file<darts_dict>(fileName);
        TestDict(deserialized);

        const lexicon_ptr &lex1 = dartsDict->get_lexicon();
        const lexicon_ptr &lex2 = deserialized->get_lexicon();

        // Compare every entry
        EXPECT_EQ(lex1->length(), lex2->length());
        for (size_t i = 0; i < lex1->length(); i++) {
            EXPECT_EQ(lex1->at(i)->key(), lex2->at(i)->key());
            EXPECT_EQ(lex1->at(i)->num_values(), lex2->at(i)->num_values());
        }

        const text_dict_ptr deserializedTextDict(new text_dict(lex2));
        TestDict(deserializedTextDict);
    }

    TEST_F(DartsDictTest, ExactMatch) {
        auto there = dartsDict->match("積羽沉舟", 12);
        EXPECT_FALSE(!there);
        auto dictEntry = *there;
        EXPECT_EQ(1, dictEntry->num_values());
        EXPECT_EQ(utf8("羣輕折軸"), dictEntry->get_default());

        auto nowhere = dartsDict->match("積羽沉舟衆口鑠金", 24);
        EXPECT_TRUE(!nowhere);
    }

} // namespace libnlp::dict
