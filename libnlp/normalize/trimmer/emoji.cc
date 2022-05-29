//
// Created by liyinbin on 2022/5/30.
//

#include "libnlp/normalize/trimmer/emoji.h"

namespace libnlp::trimmer {

    namespace {
        bool is_emoji(const std::string_view &singleCharStr) {
            char len = singleCharStr.size();
            long long num = 0;
            long long multi = 1;
            for (int i = len - 1; i >= 0 ; --i) {
                char c = singleCharStr[i];
                if (i == 0) {
                    char flag = 0x1;
                    char step = 6 - len;
                    flag = flag << step;
                    for (; step > 0; --step) {
                        flag = flag | (flag >> step);
                    }
                    c = c & flag;
                } else {
                    c = c & 0x3f;
                }

                num += c * multi;
                multi *= 64;
            }

            if (num >= 0x1f300 && num <= 0x1f6ff) {
                return true;
            } else {
                return false;
            }
        }
    }
    std::string trim_emoji(const std::string_view &str) {
        return trim_emoji(str.data(), str.size());
    }

    std::string trim_emoji(const char *cstr, size_t len) {
        std::string_view str(cstr, len);
        std::string newStr = "";
        for (size_t i = 0; i < str.length();) {
            char c = str[i];
            if (c & 0x80) {
                char flag = 0x80 >> 1;
                char count = 1;
                while ((c & flag) && (flag > 0)) {
                    count += 1;
                    flag = flag >> 1;
                }

                std::string subStr = "";
                for (char j = 0; j < count; ++j) {
                    subStr.push_back(str[i + j]);
                }

                if (is_emoji(subStr)) {
                    subStr = "";
                }

                newStr.append(subStr);
                i += count;
            } else {
                newStr.push_back(c);
                i += 1;
            }
        }

        return newStr;
    }

}