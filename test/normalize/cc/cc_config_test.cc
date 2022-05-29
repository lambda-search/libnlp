
#include <fstream>

#include "libnlp/normalize/cc/cc_config.h"
#include "cc_config_test_base.h"
#include "libnlp/normalize/cc/converter.h"
#include "libnlp/base/exception.h"
#include "test_utils_utf8.h"

namespace libnlp::cc {

    class ConfigTest : public ConfigTestBase {
    protected:
        ConfigTest()
                : input(utf8("燕燕于飞差池其羽之子于归远送于野")),
                  expected(utf8("燕燕于飛差池其羽之子于歸遠送於野")) {}

        virtual void SetUp() {
            std::cout<<"config file: "<<CONFIG_TEST_PATH<<std::endl;
            try {
                converter = config.create_from_file(CONFIG_TEST_PATH);
            } catch (const Exception &e) {
                std::cout<<e.what() << std::endl;
            }
        }

        cc_config config;
        converter_ptr converter;
        const std::string input;
        const std::string expected;
    };

    TEST_F(ConfigTest, convert) {
        const std::string &converted = converter->convert(input);
        EXPECT_EQ(expected, converted);
    }

    TEST_F(ConfigTest, ConvertBuffer) {
        char output[1024];
        const size_t length = converter->convert(input.c_str(), output);
        EXPECT_EQ(expected.length(), length);
        EXPECT_EQ(expected, output);
    }

    TEST_F(ConfigTest, NonexistingPath) {
        const std::string path = "/libnlp/no/such/file/or/directory";
        try {
            const converter_ptr _ = config.create_from_file(path);
        } catch (FileNotFound &e) {
            EXPECT_EQ(path + " not found or not accessible.", e.what());
        }
    }

    TEST_F(ConfigTest, NewFromStringWitoutTrailingSlash) {
        std::ifstream ifs(CONFIG_TEST_PATH);
        std::string content(std::istreambuf_iterator<char>(ifs),
                            (std::istreambuf_iterator<char>()));
        std::string pathWithoutTrailingSlash = TEST_DATA_ROOT
        "/config_test";

        const converter_ptr _ =
                config.create_from_string(content, pathWithoutTrailingSlash);
    }

} // namespace libnlp::cc
