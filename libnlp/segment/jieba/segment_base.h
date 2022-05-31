

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/logging.h"
#include "libnlp/segment/jieba/pre_filter.h"
#include "libnlp/segment/jieba/dict_trie.h"
#include <unordered_set>
#include <cassert>


namespace libnlp::jieba {

    const char *const SPECIAL_SEPARATORS = " \t\n\xEF\xBC\x8C\xE3\x80\x82";

    using namespace libnlp;

    class segment_base {
    public:
        segment_base() {
            XCHECK(reset_separators(SPECIAL_SEPARATORS));
        }

        virtual ~segment_base() {
        }

        virtual void cut(const string &sentence, vector<string> &words) const = 0;

        bool reset_separators(const string &s) {
            symbols_.clear();
            rune_str_array runes;
            if (!decode_runes_in_string(s, runes)) {
                XLOG(ERROR) << "decode " << s << " failed";
                return false;
            }
            for (size_t i = 0; i < runes.size(); i++) {
                if (!symbols_.insert(runes[i].rune).second) {
                    XLOG(ERROR) << s.substr(runes[i].offset, runes[i].len) << " already exists";
                    return false;
                }
            }
            return true;
        }

    protected:
        std::unordered_set<rune_t> symbols_;
    }; // class segment_base

}  // libnlp::jieba
