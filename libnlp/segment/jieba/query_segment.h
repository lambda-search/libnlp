/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include <algorithm>
#include <set>
#include <cassert>
#include "libnlp/base/logging.h"
#include "libnlp/segment/jieba/dict_trie.h"
#include "libnlp/base/unicode.h"
#include "libnlp/segment/jieba/segment_base.h"
#include "libnlp/segment/jieba/full_segment.h"
#include "libnlp/segment/jieba/mix_segment.h"

namespace libnlp::jieba {
    class query_segment : public segment_base {
    public:
        query_segment(const string &dict, const string &model, const string &userDict = "")
                : mixSeg_(dict, model, userDict),
                  trie_(mixSeg_.get_dict_trie()) {
        }

        query_segment(const dict_trie *dictTrie, const hmm_model *model)
                : mixSeg_(dictTrie, model), trie_(dictTrie) {
        }

        ~query_segment() {
        }

        void cut(const string &sentence, vector<string> &words) const {
            cut(sentence, words, true);
        }

        void cut(const string &sentence, vector<string> &words, bool hmm) const {
            vector<word_type> tmp;
            cut(sentence, tmp, hmm);
            get_strings_from_words(tmp, words);
        }

        void cut(const string &sentence, vector<word_type> &words, bool hmm = true) const {
            pre_filter pre_filter(symbols_, sentence);
            pre_filter::range range;
            vector<word_range> wrs;
            wrs.reserve(sentence.size() / 2);
            while (pre_filter.has_next()) {
                range = pre_filter.next();
                cut(range.begin, range.end, wrs, hmm);
            }
            words.clear();
            words.reserve(wrs.size());
            get_words_from_word_ranges(sentence, wrs, words);
        }

        void cut(rune_str_array::const_iterator begin, rune_str_array::const_iterator end, vector<word_range> &res,
                 bool hmm) const {
            //use mix cut first
            vector<word_range> mixRes;
            mixSeg_.cut(begin, end, mixRes, hmm);

            vector<word_range> fullRes;
            for (vector<word_range>::const_iterator mixResItr = mixRes.begin();
                 mixResItr != mixRes.end(); mixResItr++) {
                if (mixResItr->length() > 2) {
                    for (size_t i = 0; i + 1 < mixResItr->length(); i++) {
                        word_range wr(mixResItr->left + i, mixResItr->left + i + 1);
                        if (trie_->find(wr.left, wr.right + 1) != NULL) {
                            res.push_back(wr);
                        }
                    }
                }
                if (mixResItr->length() > 3) {
                    for (size_t i = 0; i + 2 < mixResItr->length(); i++) {
                        word_range wr(mixResItr->left + i, mixResItr->left + i + 2);
                        if (trie_->find(wr.left, wr.right + 1) != NULL) {
                            res.push_back(wr);
                        }
                    }
                }
                res.push_back(*mixResItr);
            }
        }

    private:
        bool is_all_ascii(const libnlp::unicode &s) const {
            for (size_t i = 0; i < s.size(); i++) {
                if (s[i] >= 0x80) {
                    return false;
                }
            }
            return true;
        }

        mix_segment mixSeg_;
        const dict_trie *trie_;
    }; // query_segment

} // namespace libnlp::jieba

