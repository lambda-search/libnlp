

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <unordered_set>
#include <cmath>
#include <limits>
#include "libnlp/base/string_util.h"
#include "libnlp/base/logging.h"
#include "libnlp/base/unicode.h"
#include "libnlp/base/std_extension.h"
#include "libnlp/segment/jieba/trie.h"

namespace libnlp::jieba {

    using namespace libnlp;

    const double MIN_DOUBLE = -3.14e+100;
    const double MAX_DOUBLE = 3.14e+100;
    const size_t DICT_COLUMN_NUM = 3;
    const char *const UNKNOWN_TAG = "";

    class dict_trie {
    public:
        enum user_word_weight_option {
            WordWeightMin,
            WordWeightMedian,
            WordWeightMax,
        }; // enum user_word_weight_option

        dict_trie(const std::string &dict_path, const std::string &user_dict_paths = "",
                 user_word_weight_option user_word_weight_opt = WordWeightMedian) {
            init(dict_path, user_dict_paths, user_word_weight_opt);
        }

        ~dict_trie() {
            delete trie_;
        }

        bool insert_user_word(const std::string &word, const std::string &tag = UNKNOWN_TAG) {
            dict_unit node_info;
            if (!make_node_info(node_info, word, user_word_default_weight_, tag)) {
                return false;
            }
            active_node_infos_.push_back(node_info);
            trie_->insert_node(node_info.word, &active_node_infos_.back());
            return true;
        }

        bool insert_user_word(const std::string &word, int freq, const std::string &tag = UNKNOWN_TAG) {
            dict_unit node_info;
            double weight = freq ? log(1.0 * freq / freq_sum_) : user_word_default_weight_;
            if (!make_node_info(node_info, word, weight, tag)) {
                return false;
            }
            active_node_infos_.push_back(node_info);
            trie_->insert_node(node_info.word, &active_node_infos_.back());
            return true;
        }

        const dict_unit *find(rune_str_array::const_iterator begin, rune_str_array::const_iterator end) const {
            return trie_->find(begin, end);
        }

        void find(rune_str_array::const_iterator begin,
                  rune_str_array::const_iterator end,
                  vector<struct dag_entity> &res,
                  size_t max_word_len = MAX_WORD_LENGTH) const {
            trie_->find(begin, end, res, max_word_len);
        }

        bool find(const std::string &word) {
            const dict_unit *tmp = NULL;
            rune_str_array runes;
            if (!decode_runes_in_string(word, runes)) {
                XLOG(ERROR) << "Decode failed.";
            }
            tmp = find(runes.begin(), runes.end());
            if (tmp == NULL) {
                return false;
            } else {
                return true;
            }
        }

        bool is_user_dict_single_chinese_word(const rune_t &word) const {
            return is_in(user_dict_single_chinese_word_, word);
        }

        double get_min_weight() const {
            return min_weight_;
        }

        void inser_user_dict_node(const std::string &line) {
            vector<string> buf;
            dict_unit node_info;
            Split(line, buf, " ");
            if (buf.size() == 1) {
                make_node_info(node_info,
                             buf[0],
                             user_word_default_weight_,
                             UNKNOWN_TAG);
            } else if (buf.size() == 2) {
                make_node_info(node_info,
                             buf[0],
                             user_word_default_weight_,
                             buf[1]);
            } else if (buf.size() == 3) {
                int freq = atoi(buf[1].c_str());
                assert(freq_sum_ > 0.0);
                double weight = log(1.0 * freq / freq_sum_);
                make_node_info(node_info, buf[0], weight, buf[2]);
            }
            static_node_infos_.push_back(node_info);
            if (node_info.word.size() == 1) {
                user_dict_single_chinese_word_.insert(node_info.word[0]);
            }
        }

        void load_user_dict(const std::vector<string> &buf) {
            for (size_t i = 0; i < buf.size(); i++) {
                inser_user_dict_node(buf[i]);
            }
        }

        void load_user_dict(const std::set<string> &buf) {
            std::set<string>::const_iterator iter;
            for (iter = buf.begin(); iter != buf.end(); iter++) {
                inser_user_dict_node(*iter);
            }
        }

        void load_user_dict(const std::string &filePaths) {
            vector<string> files = libnlp::Split(filePaths, "|;");
            size_t lineno = 0;
            for (size_t i = 0; i < files.size(); i++) {
                std::ifstream ifs(files[i].c_str());
                XCHECK(ifs.is_open()) << "open " << files[i] << " failed";
                string line;

                for (; getline(ifs, line); lineno++) {
                    if (line.size() == 0) {
                        continue;
                    }
                    inser_user_dict_node(line);
                }
            }
        }


    private:
        void init(const std::string &dict_path, const std::string &user_dict_paths, user_word_weight_option user_word_weight_opt) {
            load_dict(dict_path);
            freq_sum_ = calc_freq_sum(static_node_infos_);
            calculate_weight(static_node_infos_, freq_sum_);
            set_static_word_weights(user_word_weight_opt);

            if (user_dict_paths.size()) {
                load_user_dict(user_dict_paths);
            }
            shrink(static_node_infos_);
            CreateTrie(static_node_infos_);
        }

        void CreateTrie(const std::vector<dict_unit> &dictUnits) {
            assert(dictUnits.size());
            vector<unicode> words;
            vector<const dict_unit *> valuePointers;
            for (size_t i = 0; i < dictUnits.size(); i++) {
                words.push_back(dictUnits[i].word);
                valuePointers.push_back(&dictUnits[i]);
            }

            trie_ = new base_trie(words, valuePointers);
        }


        bool make_node_info(dict_unit &node_info,
                          const std::string &word,
                          double weight,
                          const std::string &tag) {
            if (!decode_runes_in_string(word, node_info.word)) {
                XLOG(ERROR) << "Decode " << word << " failed.";
                return false;
            }
            node_info.weight = weight;
            node_info.tag = tag;
            return true;
        }

        void load_dict(const std::string &filePath) {
            std::ifstream ifs(filePath.c_str());
            XCHECK(ifs.is_open()) << "open " << filePath << " failed.";
            string line;
            vector<string> buf;

            dict_unit node_info;
            for (size_t lineno = 0; getline(ifs, line); lineno++) {
                Split(line, buf, " ");
                XCHECK(buf.size() == DICT_COLUMN_NUM) << "split result illegal, line:" << line;
                make_node_info(node_info,
                             buf[0],
                             atof(buf[1].c_str()),
                             buf[2]);
                static_node_infos_.push_back(node_info);
            }
        }

        static bool WeightCompare(const dict_unit &lhs, const dict_unit &rhs) {
            return lhs.weight < rhs.weight;
        }

        void set_static_word_weights(user_word_weight_option option) {
            XCHECK(!static_node_infos_.empty());
            vector<dict_unit> x = static_node_infos_;
            sort(x.begin(), x.end(), WeightCompare);
            min_weight_ = x[0].weight;
            max_weight_ = x[x.size() - 1].weight;
            median_weight_ = x[x.size() / 2].weight;
            switch (option) {
                case WordWeightMin:
                    user_word_default_weight_ = min_weight_;
                    break;
                case WordWeightMedian:
                    user_word_default_weight_ = median_weight_;
                    break;
                default:
                    user_word_default_weight_ = max_weight_;
                    break;
            }
        }

        double calc_freq_sum(const std::vector<dict_unit> &node_infos) const {
            double sum = 0.0;
            for (size_t i = 0; i < node_infos.size(); i++) {
                sum += node_infos[i].weight;
            }
            return sum;
        }

        void calculate_weight(vector<dict_unit> &node_infos, double sum) const {
            assert(sum > 0.0);
            for (size_t i = 0; i < node_infos.size(); i++) {
                dict_unit &node_info = node_infos[i];
                assert(node_info.weight > 0.0);
                node_info.weight = log(double(node_info.weight) / sum);
            }
        }

        void shrink(vector<dict_unit> &units) const {
            std::vector<dict_unit>(units.begin(), units.end()).swap(units);
        }

        std::vector<dict_unit> static_node_infos_;
        std::deque<dict_unit> active_node_infos_; // must not be vector
        base_trie *trie_;

        double freq_sum_;
        double min_weight_;
        double max_weight_;
        double median_weight_;
        double user_word_default_weight_;
        std::unordered_set <rune_t> user_dict_single_chinese_word_;
    };
}  // namespace libnlp::jieba

