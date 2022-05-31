
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
#include "libnlp/segment/jieba/segment_tagged.h"
#include "libnlp/segment/jieba/pos_tagger.h"

namespace libnlp::jieba {

    class mp_segment : public segment_tagged {
    public:
        mp_segment(const std::string &dictPath, const std::string &userDictPath = "")
                : dictTrie_(new dict_trie(dictPath, userDictPath)), isNeedDestroy_(true) {
        }

        mp_segment(const dict_trie *dictTrie)
                : dictTrie_(dictTrie), isNeedDestroy_(false) {
            assert(dictTrie_);
        }

        ~mp_segment() {
            if (isNeedDestroy_) {
                delete dictTrie_;
            }
        }

        void cut(const std::string &sentence, vector<string> &words) const {
            cut(sentence, words, MAX_WORD_LENGTH);
        }

        void cut(const std::string &sentence,
                 vector<string> &words,
                 size_t max_word_len) const {
            vector<word_type> tmp;
            cut(sentence, tmp, max_word_len);
            get_strings_from_words(tmp, words);
        }

        void cut(const std::string &sentence,
                 vector<word_type> &words,
                 size_t max_word_len = MAX_WORD_LENGTH) const {
            pre_filter pre_filter(symbols_, sentence);
            pre_filter::range range;
            vector<word_range> wrs;
            wrs.reserve(sentence.size() / 2);
            while (pre_filter.has_next()) {
                range = pre_filter.next();
                cut(range.begin, range.end, wrs, max_word_len);
            }
            words.clear();
            words.reserve(wrs.size());
            get_words_from_word_ranges(sentence, wrs, words);
        }

        void cut(rune_str_array::const_iterator begin,
                 rune_str_array::const_iterator end,
                 vector<word_range> &words,
                 size_t max_word_len = MAX_WORD_LENGTH) const {
            vector<dag_entity> dags;
            dictTrie_->find(begin,
                            end,
                            dags,
                            max_word_len);
            calc_dp(dags);
            cut_by_dag(begin, end, dags, words);
        }

        const dict_trie *get_dict_trie() const {
            return dictTrie_;
        }

        bool tag(const std::string &src, std::vector<std::pair<std::string, std::string> > &res) const {
            return tagger_.tag(src, res, *this);
        }

        bool is_user_dict_single_chinese_word(const rune_t &value) const {
            return dictTrie_->is_user_dict_single_chinese_word(value);
        }

    private:
        void calc_dp(vector<dag_entity> &dags) const {
            size_t nextPos;
            const dict_unit *p;
            double val;

            for (vector<dag_entity>::reverse_iterator rit = dags.rbegin(); rit != dags.rend(); rit++) {
                rit->pInfo = NULL;
                rit->weight = MIN_DOUBLE;
                assert(!rit->nexts.empty());
                for (libnlp::inline_vector<std::pair<size_t, const dict_unit *> >::const_iterator it = rit->nexts.begin();
                     it != rit->nexts.end(); it++) {
                    nextPos = it->first;
                    p = it->second;
                    val = 0.0;
                    if (nextPos + 1 < dags.size()) {
                        val += dags[nextPos + 1].weight;
                    }

                    if (p) {
                        val += p->weight;
                    } else {
                        val += dictTrie_->get_min_weight();
                    }
                    if (val > rit->weight) {
                        rit->pInfo = p;
                        rit->weight = val;
                    }
                }
            }
        }

        void cut_by_dag(rune_str_array::const_iterator begin,
                      rune_str_array::const_iterator end,
                      const std::vector<dag_entity> &dags,
                      vector<word_range> &words) const {
            size_t i = 0;
            while (i < dags.size()) {
                const dict_unit *p = dags[i].pInfo;
                if (p) {
                    assert(p->word.size() >= 1);
                    word_range wr(begin + i, begin + i + p->word.size() - 1);
                    words.push_back(wr);
                    i += p->word.size();
                } else { //single chinese word
                    word_range wr(begin + i, begin + i);
                    words.push_back(wr);
                    i++;
                }
            }
        }

        const dict_trie *dictTrie_;
        bool isNeedDestroy_;
        pos_tagger tagger_;

    }; // class mp_segment

} // namespace libnlp::jieba
