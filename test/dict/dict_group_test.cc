
#include "dict_group_test_base.h"

namespace libnlp::dict {

    class DictGroupTest : public DictGroupTestBase {
    protected:
        DictGroupTest() {}
    };

    TEST_F(DictGroupTest, key_max_length) {
        const dict_group_ptr &dictGroup = CreateDictGroupForConversion();
        EXPECT_EQ(6, dictGroup->key_max_length());
        EXPECT_EQ(6, dictGroup->get_dicts().front()->key_max_length());
        EXPECT_EQ(3, dictGroup->get_dicts().back()->key_max_length());
    }

    TEST_F(DictGroupTest, SimpleGroupTest) {
        const dict_group_ptr &dictGroup = CreateDictGroupForConversion();
        {
            const auto &entry = dictGroup->basic_dict::match_prefix(utf8("Unknown"));
            EXPECT_TRUE(!entry);
        }
        {
            const auto &entry = dictGroup->basic_dict::match_prefix(utf8("里面"));
            EXPECT_FALSE(!entry);
            EXPECT_EQ(3, (*entry)->key_length());
            EXPECT_EQ(utf8("裏"), (*entry)->get_default());
        }
        {
            const auto &matches = dictGroup->basic_dict::match_all_prefixes(utf8("干燥"));
            EXPECT_EQ(2, matches.size());
            EXPECT_EQ(utf8("乾燥"), matches.at(0)->get_default());
            EXPECT_EQ(utf8("幹"), matches.at(1)->get_default());
        }
    }

    TEST_F(DictGroupTest, TaiwanPhraseGroupTest) {
        const dict_group_ptr dictGroup(new dict_group(
                std::list<basic_dict_ptr>{CreateDictForPhrases(), CreateTaiwanPhraseDict()}));
        {
            const auto &entry = dictGroup->basic_dict::match_prefix(utf8("鼠标"));
            EXPECT_EQ(utf8("鼠標"), (*entry)->get_default());
        }
        {
            const auto &entry = dictGroup->basic_dict::match_prefix(utf8("克罗地亚"));
            EXPECT_EQ(utf8("克羅埃西亞"), (*entry)->get_default());
        }
        {
            const auto &matches = dictGroup->basic_dict::match_all_prefixes(utf8("鼠标"));
            EXPECT_EQ(1, matches.size());
            EXPECT_EQ(utf8("鼠標"), matches[0]->get_default());
        }
    }

} // namespace libnlp::cc
