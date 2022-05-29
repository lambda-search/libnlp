/*
 * Open Chinese convert
 *
 * Copyright 2010-2014 Carbo Kuo <byvoid@byvoid.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include "libnlp/dict/dict_base.h"

namespace libnlp::dict {
    std::optional<const dict_entry *> basic_dict::match_prefix(const char *word,
                                                       size_t wordLen) const {
        long len = static_cast<long>((std::min)(key_max_length(), wordLen));
        std::string wordTrunc = UTF8Util::TruncateUTF8(word, len);
        const char *wordTruncPtr = wordTrunc.c_str() + len;
        for (; len > 0;) {
            wordTrunc.resize(static_cast<size_t>(len));
            wordTruncPtr = wordTrunc.c_str() + len;
            const std::optional<const dict_entry *> &result = match(wordTrunc.c_str());
            if (result) {
                return result;
            }
            len -= static_cast<long>(UTF8Util::PrevCharLength(wordTruncPtr));
        }
        return std::nullopt;
    }

    std::vector<const dict_entry *> basic_dict::match_all_prefixes(const char *word,
                                                          size_t wordLen) const {
        std::vector<const dict_entry *> matchedLengths;
        long len = static_cast<long>((std::min)(key_max_length(), wordLen));
        std::string wordTrunc = UTF8Util::TruncateUTF8(word, len);
        const char *wordTruncPtr = wordTrunc.c_str() + len;
        for (; len > 0;
               len -= static_cast<long>(UTF8Util::PrevCharLength(wordTruncPtr))) {
            wordTrunc.resize(static_cast<size_t>(len));
            wordTruncPtr = wordTrunc.c_str() + len;
            const std::optional<const dict_entry *> &result = match(wordTrunc.c_str());
            if (result) {
                matchedLengths.push_back(*result);
            }
        }
        return matchedLengths;
    }
}  // libnlp::dict
