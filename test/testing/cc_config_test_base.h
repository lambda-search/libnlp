
#pragma once

#include "test_utils.h"
#include <iostream>
namespace libnlp::cc {

    class ConfigTestBase : public ::testing::Test {
    protected:
        ConfigTestBase()
                : CONFIG_TEST_PATH(TEST_DATA_ROOT"/config_test/config_test.json") {
                std::cout<<CONFIG_TEST_PATH<<std::endl;
        }

        const std::string CONFIG_TEST_PATH;
    };

} // namespace libnlp::cc
