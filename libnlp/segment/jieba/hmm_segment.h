/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include <iostream>
#include <fstream>
#include <memory.h>
#include <cassert>
#include "libnlp/base/std_extension.h"
#include "libnlp/segment/jieba/hmm_model.h"
#include "libnlp/segment/jieba/segment_base.h"
#include "libnlp/segment/jieba/dict_trie.h"

namespace libnlp::jieba {
    class HMMSegment : public segment_base {
    public:
        HMMSegment(const string &filePath)
                : model_(new hmm_model(filePath)), isNeedDestroy_(true) {
        }

        HMMSegment(const hmm_model *model)
                : model_(model), isNeedDestroy_(false) {
        }

        ~HMMSegment() {
            if (isNeedDestroy_) {
                delete model_;
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

        void
        cut(rune_str_array::const_iterator begin, rune_str_array::const_iterator end, vector<word_range> &res) const {
            rune_str_array::const_iterator left = begin;
            rune_str_array::const_iterator right = begin;
            while (right != end) {
                if (right->rune < 0x80) {
                    if (left != right) {
                        InternalCut(left, right, res);
                    }
                    left = right;
                    do {
                        right = SequentialLetterRule(left, end);
                        if (right != left) {
                            break;
                        }
                        right = NumbersRule(left, end);
                        if (right != left) {
                            break;
                        }
                        right++;
                    } while (false);
                    word_range wr(left, right - 1);
                    res.push_back(wr);
                    left = right;
                } else {
                    right++;
                }
            }
            if (left != right) {
                InternalCut(left, right, res);
            }
        }

    private:
        // sequential letters rule
        rune_str_array::const_iterator
        SequentialLetterRule(rune_str_array::const_iterator begin, rune_str_array::const_iterator end) const {
            rune_t x = begin->rune;
            if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z')) {
                begin++;
            } else {
                return begin;
            }
            while (begin != end) {
                x = begin->rune;
                if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z') || ('0' <= x && x <= '9')) {
                    begin++;
                } else {
                    break;
                }
            }
            return begin;
        }

        //
        rune_str_array::const_iterator
        NumbersRule(rune_str_array::const_iterator begin, rune_str_array::const_iterator end) const {
            rune_t x = begin->rune;
            if ('0' <= x && x <= '9') {
                begin++;
            } else {
                return begin;
            }
            while (begin != end) {
                x = begin->rune;
                if (('0' <= x && x <= '9') || x == '.') {
                    begin++;
                } else {
                    break;
                }
            }
            return begin;
        }

        void InternalCut(rune_str_array::const_iterator begin, rune_str_array::const_iterator end,
                         vector<word_range> &res) const {
            vector<size_t> status;
            Viterbi(begin, end, status);

            rune_str_array::const_iterator left = begin;
            rune_str_array::const_iterator right;
            for (size_t i = 0; i < status.size(); i++) {
                if (status[i] % 2) { //if (hmm_model::E == status[i] || hmm_model::S == status[i])
                    right = begin + i + 1;
                    word_range wr(left, right - 1);
                    res.push_back(wr);
                    left = right;
                }
            }
        }

        void Viterbi(rune_str_array::const_iterator begin,
                     rune_str_array::const_iterator end,
                     vector<size_t> &status) const {
            size_t Y = hmm_model::STATUS_SUM;
            size_t X = end - begin;

            size_t XYSize = X * Y;
            size_t now, old, stat;
            double tmp, endE, endS;

            vector<int> path(XYSize);
            vector<double> weight(XYSize);

            //start
            for (size_t y = 0; y < Y; y++) {
                weight[0 + y * X] =
                        model_->startProb[y] + model_->get_emit_prob(model_->emitProbVec[y], begin->rune, MIN_DOUBLE);
                path[0 + y * X] = -1;
            }

            double emitProb;

            for (size_t x = 1; x < X; x++) {
                for (size_t y = 0; y < Y; y++) {
                    now = x + y * X;
                    weight[now] = MIN_DOUBLE;
                    path[now] = hmm_model::E; // warning
                    emitProb = model_->get_emit_prob(model_->emitProbVec[y], (begin + x)->rune, MIN_DOUBLE);
                    for (size_t preY = 0; preY < Y; preY++) {
                        old = x - 1 + preY * X;
                        tmp = weight[old] + model_->transProb[preY][y] + emitProb;
                        if (tmp > weight[now]) {
                            weight[now] = tmp;
                            path[now] = preY;
                        }
                    }
                }
            }

            endE = weight[X - 1 + hmm_model::E * X];
            endS = weight[X - 1 + hmm_model::S * X];
            stat = 0;
            if (endE >= endS) {
                stat = hmm_model::E;
            } else {
                stat = hmm_model::S;
            }

            status.resize(X);
            for (int x = X - 1; x >= 0; x--) {
                status[x] = stat;
                stat = path[x + stat * X];
            }
        }

        const hmm_model *model_;
        bool isNeedDestroy_;
    }; // class HMMSegment

} // namespace libnlp::jieba

