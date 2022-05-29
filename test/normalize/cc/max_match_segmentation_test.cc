
#include "libnlp/normalize/cc/max_match_segmentation.h"
#include "dict_group_test_base.h"
#include "libnlp/normalize/cc/segments.h"

namespace libnlp::cc {

    class MaxMatchSegmentationTest : public libnlp::dict::DictGroupTestBase {
    protected:
        MaxMatchSegmentationTest() {}

        virtual void SetUp() {
            _dict = CreateDictGroupForConversion();
            _segmenter = segmentation_ptr(new max_match_segmentation(_dict));
        }

        libnlp::dict::basic_dict_ptr _dict;
        segmentation_ptr _segmenter;
    };

    TEST_F(MaxMatchSegmentationTest, segment) {
        const auto &segments = _segmenter->segment(utf8("太后的头发干燥"));
        EXPECT_EQ(4, segments->length());
        EXPECT_EQ(utf8("太后"), std::string(segments->at(0)));
        EXPECT_EQ(utf8("的"), std::string(segments->at(1)));
        EXPECT_EQ(utf8("头发"), std::string(segments->at(2)));
        EXPECT_EQ(utf8("干燥"), std::string(segments->at(3)));
    }

} // namespace libnlp::cc
