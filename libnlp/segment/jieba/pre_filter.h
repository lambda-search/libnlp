

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <unordered_set>
#include "libnlp/base/std_extension.h"
#include "libnlp/segment/jieba/trie.h"
#include "libnlp/base/logging.h"

namespace libnlp::jieba {
    using namespace libnlp;

    class pre_filter {
    public:
        //TODO use word_range instead of Range
        struct range {
            rune_str_array::const_iterator begin;
            rune_str_array::const_iterator end;
        }; // struct Range

        pre_filter(const std::unordered_set<rune_t> &symbols,
                   const string &sentence)
                : symbols_(symbols) {
            if (!decode_runes_in_string(sentence, sentence_)) {
                XLOG(ERROR) << "decode failed. ";
            }
            cursor_ = sentence_.begin();
        }

        ~pre_filter() {
        }

        bool has_next() const {
            return cursor_ != sentence_.end();
        }

        range next() {
            range range;
            range.begin = cursor_;
            while (cursor_ != sentence_.end()) {
                if (is_in(symbols_, cursor_->rune)) {
                    if (range.begin == cursor_) {
                        cursor_++;
                    }
                    range.end = cursor_;
                    return range;
                }
                cursor_++;
            }
            range.end = sentence_.end();
            return range;
        }

    private:
        rune_str_array::const_iterator cursor_;
        rune_str_array sentence_;
        const std::unordered_set<rune_t> &symbols_;
    }; // class pre_filter

}  // namespace libnlp::jieba

