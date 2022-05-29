
#include <cmath>

#include "libnlp/normalize/cc/phrase_extract.h"
#include "test_utils.h"
#include "test_utils_utf8.h"

namespace libnlp::cc {

    typedef phrase_extract::UTF8StringSlice8Bit UTF8StringSlice8Bit;

    class PhraseExtractTest : public ::testing::Test {
    protected:
        PhraseExtractTest()
                : _siShi(utf8("四是四十是十十四是十四四十是四十")),
                  _punctuation(utf8("一.二.三")) {}

        const std::vector<UTF8StringSlice8Bit> &Suffixes() const {
            return _phraseExtract._suffixes;
        }

        const std::vector<UTF8StringSlice8Bit> &Prefixes() const {
            return _phraseExtract._prefixes;
        }

        phrase_extract _phraseExtract;

        const std::string _siShi;
        const std::string _punctuation;
    };

    TEST_F(PhraseExtractTest, extract_suffixes) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.extract_suffixes();
        EXPECT_EQ(
                std::vector<UTF8StringSlice8Bit>(
                {
                    "十", "十十四是", "十四四十", "十四是十", "十是十十", "十是四十",
                            "四十", "四十是十", "四十是四", "四四十是", "四是十四", "四是四十",
                            "是十十四", "是十四四", "是四十", "是四十是"
                }),
                Suffixes());
    }

    TEST_F(PhraseExtractTest, extract_prefixes) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.extract_prefixes();
        EXPECT_EQ(
                std::vector<UTF8StringSlice8Bit>(
                {
                    "十是十十", "十四四十", "十是四十", "四是四十", "四十是十",
                            "十四是十", "四", "是十十四", "四是十四", "是十四四", "四十是四",
                            "四是四", "四四十是", "是四十是", "四是", "十十四是"
                }),
                Prefixes());
    }

    TEST_F(PhraseExtractTest, calculate_frequency) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.calculate_frequency();
        EXPECT_EQ(6, _phraseExtract.frequency("四"));
        EXPECT_EQ(6, _phraseExtract.frequency("十"));
        EXPECT_EQ(4, _phraseExtract.frequency("是"));
        EXPECT_EQ(3, _phraseExtract.frequency("四十"));
        EXPECT_EQ(2, _phraseExtract.frequency("是四十"));
        EXPECT_EQ(2, _phraseExtract.frequency("是四"));
        EXPECT_EQ(2, _phraseExtract.frequency("四是"));
        EXPECT_DOUBLE_EQ(-2.0149030205422647, _phraseExtract.log_probability("四"));
        EXPECT_DOUBLE_EQ(-2.0149030205422647, _phraseExtract.log_probability("十"));
        EXPECT_DOUBLE_EQ(-2.4203681286504288, _phraseExtract.log_probability("是"));
        EXPECT_DOUBLE_EQ(-2.7080502011022096, _phraseExtract.log_probability("四十"));
        EXPECT_DOUBLE_EQ(-3.8066624897703196, _phraseExtract.log_probability("是十十"));
    }

    TEST_F(PhraseExtractTest, extract_word_candidates) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.extract_word_candidates();
        EXPECT_EQ(std::vector<UTF8StringSlice8Bit>(
                          {
                              "十", "四", "是", "四十", "十四", "十是",
                                      "四十是", "四是", "是十", "是四", "是四十", "十十",
                                      "十十四", "十四四", "十四是", "十是十", "十是四", "四四",
                                      "四四十", "四是十", "四是四", "是十十", "是十四"
                          }),
                  _phraseExtract.word_candidates());
    }

    TEST_F(PhraseExtractTest, calculate_cohesions) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.calculate_cohesions();
        EXPECT_DOUBLE_EQ(INFINITY, _phraseExtract.cohesion("四"));
        EXPECT_DOUBLE_EQ(1.3217558399823193, _phraseExtract.cohesion("四十"));
        EXPECT_DOUBLE_EQ(0.91629073187415511, _phraseExtract.cohesion("十四"));
        EXPECT_DOUBLE_EQ(1.3217558399823193, _phraseExtract.cohesion("十是"));
        EXPECT_DOUBLE_EQ(1.3217558399823193, _phraseExtract.cohesion("四是四"));
        EXPECT_DOUBLE_EQ(1.3217558399823193, _phraseExtract.cohesion("十是十"));
    }

    TEST_F(PhraseExtractTest, calculate_suffi_entropy) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.calculate_suffi_entropy();
        EXPECT_DOUBLE_EQ(1.0549201679861442, _phraseExtract.suffix_entropy("十"));
        EXPECT_DOUBLE_EQ(1.0114042647073518, _phraseExtract.suffix_entropy("四"));
        EXPECT_DOUBLE_EQ(0.69314718055994529, _phraseExtract.suffix_entropy("十四"));
        EXPECT_DOUBLE_EQ(0.69314718055994529, _phraseExtract.suffix_entropy("十是"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.suffix_entropy("四十"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.suffix_entropy("四是四"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.suffix_entropy("十是十"));
    }

    TEST_F(PhraseExtractTest, calculate_prefix_entropy) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.calculate_prefix_entropy();
        EXPECT_DOUBLE_EQ(1.0114042647073516, _phraseExtract.prefix_entropy("十"));
        EXPECT_DOUBLE_EQ(1.0549201679861442, _phraseExtract.prefix_entropy("四"));
        EXPECT_DOUBLE_EQ(0.69314718055994529, _phraseExtract.prefix_entropy("十四"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.prefix_entropy("十是"));
        EXPECT_DOUBLE_EQ(0.63651416829481278, _phraseExtract.prefix_entropy("四十"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.prefix_entropy("四是四"));
        EXPECT_DOUBLE_EQ(0, _phraseExtract.prefix_entropy("十是十"));
    }

    TEST_F(PhraseExtractTest, select_words) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(3);
        _phraseExtract.set_full_text(_siShi);
        _phraseExtract.set_post_calculation_filter(
                [](const phrase_extract &_phraseExtract, const UTF8StringSlice8Bit &word) {
                    return _phraseExtract.frequency(word) == 1;
                });
        _phraseExtract.select_words();
        EXPECT_EQ(std::vector<UTF8StringSlice8Bit>({"十", "四", "是", "四十", "十四",
                                                    "十是", "四十是", "四是", "是十",
                                                    "是四", "是四十"}),
                  _phraseExtract.words());
    }

    TEST_F(PhraseExtractTest, Punctuation) {
        _phraseExtract.reset();
        _phraseExtract.set_word_min_length(1);
        _phraseExtract.set_word_max_length(2);
        _phraseExtract.set_full_text(_punctuation);
        _phraseExtract.extract_prefixes();
        EXPECT_EQ(
                std::vector<UTF8StringSlice8Bit>({"一.", ".二.", "一", "二.三", "一.二"}),
                Prefixes());
    }

} // namespace libnlp::cc
