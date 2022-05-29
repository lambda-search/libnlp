
#include <thread>

#include "cc_config_test_base.h"
#include "libnlp/normalize/cc/simple_converter.h"
#include "test_utils_utf8.h"
#include "libnlp/normalize/cc/libnlp_cc.h"

namespace libnlp::cc {

    class SimpleConverterTest : public ConfigTestBase {
    protected:
        SimpleConverterTest() {}

        void TestConverter(const std::string &config) const {
            const simple_converter converter(config);
            const std::string &converted =
                    converter.convert(utf8("燕燕于飞差池其羽之子于归远送于野"));
            EXPECT_EQ(utf8("燕燕于飛差池其羽之子于歸遠送於野"), converted);
        }
    };

    TEST_F(SimpleConverterTest, convert) { TestConverter(CONFIG_TEST_PATH); }

    TEST_F(SimpleConverterTest, Multithreading) {
        const auto &routine = [this](const std::string &config) {
            TestConverter(config);
        };
        std::thread thread1(routine, CONFIG_TEST_PATH);
        std::thread thread2(routine, CONFIG_TEST_PATH);
        routine(CONFIG_TEST_PATH);
        thread1.join();
        thread2.join();
    }

    TEST_F(SimpleConverterTest, CInterface) {
        const std::string &text = utf8("燕燕于飞差池其羽之子于归远送于野");
        const std::string &expected = utf8("燕燕于飛差池其羽之子于歸遠送於野");
        {
            nlpcc_t od = nlp_cc_open(CONFIG_TEST_PATH.c_str());
            char *converted = nlp_cc_convert_utf8(od, text.c_str(), (size_t) -1);
            EXPECT_EQ(expected, converted);
            nlp_cc_convert_utf8_free(converted);
            EXPECT_EQ(0, nlp_cc_close(od));
        }
        {
            char output[1024];
            nlpcc_t od = nlp_cc_open(CONFIG_TEST_PATH.c_str());
            size_t length =
                    nlp_cc_convert_utf8_to_buffer(od, text.c_str(), (size_t) -1, output);
            EXPECT_EQ(expected.length(), length);
            EXPECT_EQ(expected, output);
            EXPECT_EQ(0, nlp_cc_close(od));
        }
        {
            std::string path = "/libnlp/no/such/file/or/directory";
            nlpcc_t od = nlp_cc_open(path.c_str());
            EXPECT_EQ(reinterpret_cast<nlpcc_t>(-1), od);
            EXPECT_EQ(path + " not found or not accessible.", nlp_cc_error());
        }
    }

} // namespace libnlp::cc
