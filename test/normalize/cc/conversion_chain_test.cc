

#include "libnlp/normalize/cc/conversion_chain.h"
#include "libnlp/normalize/cc/segments.h"
#include "dict_group_test_base.h"

namespace libnlp::cc {

    class ConversionChainTest : public libnlp::dict::DictGroupTestBase {
    protected:
        ConversionChainTest() {}

        virtual void SetUp() {
            _dict = CreateDictGroupForConversion();
            _conversion = conversion_ptr(new conversion(_dict));
        }

        void SegmentsAssertEquals(const segments_ptr &expected,
                                  const segments_ptr &actual) {
            const size_t length = expected->length();
            EXPECT_TRUE(length == actual->length());
            for (size_t i = 0; i < length; i++) {
                EXPECT_EQ(std::string(expected->at(i)), std::string(actual->at(i)))
                                    << "i = " << i;
            }
        }

        libnlp::dict::basic_dict_ptr _dict;
        conversion_ptr _conversion;
    };

    TEST_F(ConversionChainTest, convert) {
        // Variants
        const libnlp::dict::basic_dict_ptr &dictVariants = CreateDictForTaiwanVariants();
        const conversion_ptr &conversionVariants =
                conversion_ptr(new conversion(dictVariants));
        const std::list<conversion_ptr> conversions{_conversion, conversionVariants};
        const conversion_chain_ptr &conversionChain =
                conversion_chain_ptr(new conversion_chain(conversions));
        const segments_ptr &converted =
                conversionChain->convert(segments_ptr(new segments{utf8("里面")}));
        SegmentsAssertEquals(segments_ptr(new segments{utf8("裡面")}), converted);
    }

} // namespace libnlp::cc
