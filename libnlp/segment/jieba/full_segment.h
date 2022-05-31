

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
#include "libnlp/base/unicode.h"
#include "libnlp/segment/jieba/dict_trie.h"
#include "libnlp/segment/jieba/segment_base.h"

namespace libnlp::jieba {

    class full_segment : public segment_base {
    public:
        full_segment(const string &dictPath) {
            dictTrie_ = new dict_trie(dictPath);
            isNeedDestroy_ = true;
        }

        full_segment(const dict_trie *dictTrie)
                : dictTrie_(dictTrie), isNeedDestroy_(false) {
            assert(dictTrie_);
        }

        ~full_segment() {
            if (isNeedDestroy_) {
                delete dictTrie_;
            }
        }

        void cut(const string &sentence,
                 vector<string> &words) const {
            vector<word_type> tmp;
            cut(sentence, tmp);
            get_strings_from_words(tmp, words);
        }

        void cut(const string &sentence,
                 vector<word_type> &words) const {
            pre_filter pre_filter(symbols_, sentence);
            pre_filter::range range;
            vector<word_range> wrs;
            wrs.reserve(sentence.size() / 2);
            while (pre_filter.has_next()) {
                range = pre_filter.next();
                cut(range.begin, range.end, wrs);
            }
            words.clear();
            words.reserve(wrs.size());
            get_words_from_word_ranges(sentence, wrs, words);
        }

        void cut(rune_str_array::const_iterator begin,
                 rune_str_array::const_iterator end,
                 vector<word_range> &res) const {
            // result of searching in trie tree
            inline_vector<pair<size_t, const dict_unit *>> tRes;

            // max index of res's words
            size_t maxIdx = 0;

            // always equals to (uItr - begin)
            size_t uIdx = 0;

            // tmp variables
            size_t wordLen = 0;
            assert(dictTrie_);
            vector<struct dag_entity> dags;
            dictTrie_->find(begin, end, dags);
            for (size_t i = 0; i < dags.size(); i++) {
                for (size_t j = 0; j < dags[i].nexts.size(); j++) {
                    size_t nextoffset = dags[i].nexts[j].first;
                    assert(nextoffset < dags.size());
                    const dict_unit *du = dags[i].nexts[j].second;
                    if (du == NULL) {
                        if (dags[i].nexts.size() == 1 && maxIdx <= uIdx) {
                            word_range wr(begin + i, begin + nextoffset);
                            res.push_back(wr);
                        }
                    } else {
                        wordLen = du->word.size();
                        if (wordLen >= 2 || (dags[i].nexts.size() == 1 && maxIdx <= uIdx)) {
                            word_range wr(begin + i, begin + nextoffset);
                            res.push_back(wr);
                        }
                    }
                    maxIdx = uIdx + wordLen > maxIdx ? uIdx + wordLen : maxIdx;
                }
                uIdx++;
            }
        }

    private:
        const dict_trie *dictTrie_;
        bool isNeedDestroy_;
    };
}

