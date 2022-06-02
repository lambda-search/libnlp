
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/


#include <algorithm>
#include "libnlp/dict/dict_base.h"

namespace libnlp::dict {
    std::optional<const dict_entity *> basic_dict::match_prefix(const char *word,
                                                                size_t wordLen) const {
        long len = static_cast<long>((std::min)(key_max_length(), wordLen));
        std::string wordTrunc = UTF8Util::TruncateUTF8(word, len);
        const char *wordTruncPtr = wordTrunc.c_str() + len;
        for (; len > 0;) {
            wordTrunc.resize(static_cast<size_t>(len));
            wordTruncPtr = wordTrunc.c_str() + len;
            const std::optional<const dict_entity *> &result = match(wordTrunc.c_str());
            if (result) {
                return result;
            }
            len -= static_cast<long>(UTF8Util::PrevCharLength(wordTruncPtr));
        }
        return std::nullopt;
    }

    std::vector<const dict_entity *> basic_dict::match_all_prefixes(const char *word,
                                                                    size_t wordLen) const {
        std::vector<const dict_entity *> matchedLengths;
        long len = static_cast<long>((std::min)(key_max_length(), wordLen));
        std::string wordTrunc = UTF8Util::TruncateUTF8(word, len);
        const char *wordTruncPtr = wordTrunc.c_str() + len;
        for (; len > 0;
               len -= static_cast<long>(UTF8Util::PrevCharLength(wordTruncPtr))) {
            wordTrunc.resize(static_cast<size_t>(len));
            wordTruncPtr = wordTrunc.c_str() + len;
            const std::optional<const dict_entity *> &result = match(wordTrunc.c_str());
            if (result) {
                matchedLengths.push_back(*result);
            }
        }
        return matchedLengths;
    }
}  // libnlp::dict
