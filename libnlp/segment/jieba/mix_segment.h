/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <cassert>
#include "libnlp/segment/jieba/mp_segment.h"
#include "libnlp/segment/jieba/hmm_segment.h"
#include "libnlp/base/string_util.h"
#include "libnlp/segment/jieba/pos_tagger.h"

namespace libnlp::jieba {
    class mix_segment : public segment_tagged {
    public:
        mix_segment(const std::string &mpSegDict, const std::string &hmmSegDict,
                    const std::string &userDict = "")
                : mpSeg_(mpSegDict, userDict),
                  hmmSeg_(hmmSegDict) {
        }

        mix_segment(const dict_trie *dictTrie, const hmm_model *model)
                : mpSeg_(dictTrie), hmmSeg_(model) {
        }

        ~mix_segment() {
        }

        void cut(const std::string &sentence, vector<string> &words) const {
            cut(sentence, words, true);
        }

        void cut(const std::string &sentence, vector<string> &words, bool hmm) const {
            vector<word_type> tmp;
            cut(sentence, tmp, hmm);
            get_strings_from_words(tmp, words);
        }

        void cut(const std::string &sentence, vector<word_type> &words, bool hmm = true) const {
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
            if (!hmm) {
                mpSeg_.cut(begin, end, res);
                return;
            }
            vector<word_range> words;
            assert(end >= begin);
            words.reserve(end - begin);
            mpSeg_.cut(begin, end, words);

            vector<word_range> hmmRes;
            hmmRes.reserve(end - begin);
            for (size_t i = 0; i < words.size(); i++) {
                //if mp Get a word, it's ok, put it into result
                if (words[i].left != words[i].right ||
                    (words[i].left == words[i].right && mpSeg_.is_user_dict_single_chinese_word(words[i].left->rune))) {
                    res.push_back(words[i]);
                    continue;
                }

                // if mp Get a single one and it is not in userdict, collect it in sequence
                size_t j = i;
                while (j < words.size() && words[j].left == words[j].right &&
                       !mpSeg_.is_user_dict_single_chinese_word(words[j].left->rune)) {
                    j++;
                }

                // cut the sequence with hmm
                assert(j - 1 >= i);
                // TODO
                hmmSeg_.cut(words[i].left, words[j - 1].left + 1, hmmRes);
                //put hmm result to result
                for (size_t k = 0; k < hmmRes.size(); k++) {
                    res.push_back(hmmRes[k]);
                }

                //clear tmp vars
                hmmRes.clear();

                //let i jump over this piece
                i = j - 1;
            }
        }

        const dict_trie *get_dict_trie() const {
            return mpSeg_.get_dict_trie();
        }

        bool tag(const std::string &src, std::vector<std::pair<std::string, std::string> > &res) const {
            return tagger_.tag(src, res, *this);
        }

        string lookup_tag(const std::string &str) const {
            return tagger_.lookup_tag(str, *this);
        }

    private:
        mp_segment mpSeg_;
        hmm_segment hmmSeg_;
        pos_tagger tagger_;

    }; // class mix_segment

} // namespace libnlp::jieba

