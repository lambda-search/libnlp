
#include "test_utils_utf8.h"
#include "text_dict_test_base.h"

namespace libnlp::dict {

    class TextDictTest : public TextDictTestBase {
    protected:
        TextDictTest() : fileName("dict.txt") {};

        const std::string fileName;
    };

    TEST_F(TextDictTest, DictTest) { TestDict(textDict); }

    TEST_F(TextDictTest, Serialization) {
        textDict->libnlp::dict::serializable_dict::serialize_to_file(fileName);
    }

    TEST_F(TextDictTest, Deserialization) {
        const text_dict_ptr &deserialized =
                serializable_dict::create_from_file<text_dict>(fileName);
        TestDict(deserialized);
    }

    TEST_F(TextDictTest, ExactMatch) {
        auto there = textDict->match("積羽沉舟", 12);
        EXPECT_FALSE(!there);
        auto dictEntry = (*there);
        EXPECT_EQ(1, dictEntry->num_values());
        EXPECT_EQ(utf8("羣輕折軸"), dictEntry->get_default());

        auto nowhere = textDict->match("積羽沉舟衆口鑠金", 24);
        EXPECT_TRUE(!nowhere);
    }

    TEST_F(TextDictTest, match_prefix) {
        {
            auto there = textDict->match_prefix("清華", 3);
            EXPECT_FALSE(!there);
            auto dictEntry = (*there);
            EXPECT_EQ(utf8("Tsing"), dictEntry->get_default());
        }
        {
            auto there = textDict->match_prefix("清華", 5);
            EXPECT_FALSE(!there);
            auto dictEntry = (*there);
            EXPECT_EQ(utf8("Tsing"), dictEntry->get_default());
        }
        {
            auto there = textDict->match_prefix("清華", 6);
            EXPECT_FALSE(!there);
            auto dictEntry = (*there);
            EXPECT_EQ(utf8("Tsinghua"), dictEntry->get_default());
        }
        {
            auto there = textDict->match_prefix("清華", 100);
            EXPECT_FALSE(!there);
            auto dictEntry = (*there);
            EXPECT_EQ(utf8("Tsinghua"), dictEntry->get_default());
        }
    }

} // namespace libnlp::cc
