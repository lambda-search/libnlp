
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include "libnlp/dict/serialized_values.h"
#include "text_dict_test_base.h"

namespace libnlp::cc {

    class SerializedValuesTest : public libnlp::dict::TextDictTestBase {
    protected:
        SerializedValuesTest()
                : binDict(new libnlp::dict::serialized_values(textDict->get_lexicon())),
                  fileName("dict.bin") {};

        const std::shared_ptr<libnlp::dict::serialized_values> binDict;
        const std::string fileName;
    };

    TEST_F(SerializedValuesTest, Serialization) {
        binDict->libnlp::dict::serializable_dict::serialize_to_file(fileName);
    }

    TEST_F(SerializedValuesTest, Deserialization) {
        const std::shared_ptr<libnlp::dict::serialized_values> &deserialized =
                libnlp::dict::serializable_dict::create_from_file<libnlp::dict::serialized_values>(fileName);
        const libnlp::dict::lexicon_ptr &lex1 = binDict->get_lexicon();
        const libnlp::dict::lexicon_ptr &lex2 = deserialized->get_lexicon();

        // Compare every entry
        EXPECT_EQ(lex1->length(), lex2->length());
        for (size_t i = 0; i < lex1->length(); i++) {
            EXPECT_EQ(lex1->at(i)->num_values(), lex2->at(i)->num_values());
        }
    }

} // namespace libnlp::cc
