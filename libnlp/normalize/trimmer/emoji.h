//
// Created by liyinbin on 2022/5/30.
//

#pragma once

#include <string>
#include <string_view>

namespace libnlp::trimmer {

    std::string trim_emoji(const std::string_view &str);

    std::string trim_emoji(const char *str, size_t len);
}
