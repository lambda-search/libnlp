/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include <string>
#include <vector>
#include "libnlp/segment/jieba/query_segment.h"
#include "libnlp/segment/jieba/keyword_extractor.h"

namespace libnlp::jieba {

    class jieba_engine {
    public:
        jieba_engine(const std::string &dict_path,
                  const std::string &model_path,
                  const std::string &user_dict_path,
                  const std::string &idfPath,
                  const std::string &stopWordPath)
                : dict_trie_(dict_path, user_dict_path),
                  model_(model_path),
                  mp_seg_(&dict_trie_),
                  hmm_seg_(&model_),
                  mix_seg_(&dict_trie_, &model_),
                  full_seg_(&dict_trie_),
                  query_seg_(&dict_trie_, &model_),
                  extractor(&dict_trie_, &model_, idfPath, stopWordPath) {
        }

        ~jieba_engine() {
        }

        struct loc_word {
            std::string word;
            size_t begin;
            size_t end;
        }; // struct loc_word

        void cut(const std::string &sentence, vector<std::string> &words, bool hmm = true) const {
            mix_seg_.cut(sentence, words, hmm);
        }

        void cut(const std::string &sentence, vector<word_type> &words, bool hmm = true) const {
            mix_seg_.cut(sentence, words, hmm);
        }

        void cut_all(const std::string &sentence, std::vector<std::string> &words) const {
            full_seg_.cut(sentence, words);
        }

        void cut_all(const std::string &sentence, std::vector<word_type> &words) const {
            full_seg_.cut(sentence, words);
        }

        void cut_for_search(const std::string &sentence, std::vector<std::string> &words, bool hmm = true) const {
            query_seg_.cut(sentence, words, hmm);
        }

        void cut_for_search(const std::string &sentence, std::vector<word_type> &words, bool hmm = true) const {
            query_seg_.cut(sentence, words, hmm);
        }

        void cut_hmm(const std::string &sentence, std::vector<std::string> &words) const {
            hmm_seg_.cut(sentence, words);
        }

        void cut_hmm(const std::string &sentence, std::vector<word_type> &words) const {
            hmm_seg_.cut(sentence, words);
        }

        void cut_small(const std::string &sentence, std::vector<std::string> &words, size_t max_word_len) const {
            mp_seg_.cut(sentence, words, max_word_len);
        }

        void cut_small(const std::string &sentence, std::vector<word_type> &words, size_t max_word_len) const {
            mp_seg_.cut(sentence, words, max_word_len);
        }

        void tag(const std::string &sentence, std::vector<std::pair<std::string, std::string> > &words) const {
            mix_seg_.tag(sentence, words);
        }

        std::string lookup_tag(const std::string &str) const {
            return mix_seg_.lookup_tag(str);
        }

        bool insert_user_word(const std::string &word, const std::string &tag = UNKNOWN_TAG) {
            return dict_trie_.insert_user_word(word, tag);
        }

        bool insert_user_word(const std::string &word, int freq, const std::string &tag = UNKNOWN_TAG) {
            return dict_trie_.insert_user_word(word, freq, tag);
        }

        bool Find(const std::string &word) {
            return dict_trie_.find(word);
        }

        void reset_separators(const std::string &s) {
            //TODO
            mp_seg_.reset_separators(s);
            hmm_seg_.reset_separators(s);
            mix_seg_.reset_separators(s);
            full_seg_.reset_separators(s);
            query_seg_.reset_separators(s);
        }

        const dict_trie *get_dict_trie() const {
            return &dict_trie_;
        }

        const hmm_model *get_hmm_model() const {
            return &model_;
        }

        void load_user_dict(const std::vector<string> &buf) {
            dict_trie_.load_user_dict(buf);
        }

        void load_user_dict(const std::set<string> &buf) {
            dict_trie_.load_user_dict(buf);
        }

        void load_user_dict(const std::string &path) {
            dict_trie_.load_user_dict(path);
        }

    private:
        dict_trie dict_trie_;
        hmm_model model_;

        // They share the same dict trie and model
        mp_segment mp_seg_;
        hmm_segment hmm_seg_;
        mix_segment mix_seg_;
        full_segment full_seg_;
        query_segment query_seg_;

    public:
        keyword_extractor extractor;
    }; // class jieba_engine

} // namespace libnlp::jieba

