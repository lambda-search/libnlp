
#include "libnlp/normalize/cc/conversion.h"
#include "dict_group_test_base.h"

namespace libnlp::cc {

    class ConversionTest : public libnlp::dict::DictGroupTestBase {
    protected:
        ConversionTest()
                : _input(utf8("太后的头发干燥")), _expected(utf8("太后的頭髮乾燥")) {}

        virtual void SetUp() {
            _dict = CreateDictGroupForConversion();
            _conversion = conversion_ptr(new conversion(_dict));
        }

        libnlp::dict::basic_dict_ptr _dict;
        conversion_ptr _conversion;
        const std::string _input;
        const std::string _expected;
    };

    TEST_F(ConversionTest, ConvertString) {
        const std::string converted = _conversion->convert(_input);
        EXPECT_EQ(_expected, converted);
    }

    TEST_F(ConversionTest, ConvertCString) {
        const std::string converted = _conversion->convert(_input.c_str());
        EXPECT_EQ(_expected, converted);
    }

} // namespace libnlp::cc
