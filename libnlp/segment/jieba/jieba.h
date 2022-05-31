/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include "libnlp/segment/jieba/query_segment.h"
#include "libnlp/segment/jieba/keyword_extractor.h"

namespace libnlp::jieba {

    class jieba_engine {
    public:
        jieba_engine(const string &dict_path,
                  const string &model_path,
                  const string &user_dict_path,
                  const string &idfPath,
                  const string &stopWordPath)
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
            string word;
            size_t begin;
            size_t end;
        }; // struct loc_word

        void cut(const string &sentence, vector<string> &words, bool hmm = true) const {
            mix_seg_.cut(sentence, words, hmm);
        }

        void cut(const string &sentence, vector<word_type> &words, bool hmm = true) const {
            mix_seg_.cut(sentence, words, hmm);
        }

        void cut_all(const string &sentence, vector<string> &words) const {
            full_seg_.cut(sentence, words);
        }

        void cut_all(const string &sentence, vector<word_type> &words) const {
            full_seg_.cut(sentence, words);
        }

        void cut_for_search(const string &sentence, vector<string> &words, bool hmm = true) const {
            query_seg_.cut(sentence, words, hmm);
        }

        void cut_for_search(const string &sentence, vector<word_type> &words, bool hmm = true) const {
            query_seg_.cut(sentence, words, hmm);
        }

        void cut_hmm(const string &sentence, vector<string> &words) const {
            hmm_seg_.cut(sentence, words);
        }

        void cut_hmm(const string &sentence, vector<word_type> &words) const {
            hmm_seg_.cut(sentence, words);
        }

        void cut_small(const string &sentence, vector<string> &words, size_t max_word_len) const {
            mp_seg_.cut(sentence, words, max_word_len);
        }

        void cut_small(const string &sentence, vector<word_type> &words, size_t max_word_len) const {
            mp_seg_.cut(sentence, words, max_word_len);
        }

        void tag(const string &sentence, vector<pair<string, string> > &words) const {
            mix_seg_.tag(sentence, words);
        }

        string lookup_tag(const string &str) const {
            return mix_seg_.lookup_tag(str);
        }

        bool insert_user_word(const string &word, const string &tag = UNKNOWN_TAG) {
            return dict_trie_.insert_user_word(word, tag);
        }

        bool insert_user_word(const string &word, int freq, const string &tag = UNKNOWN_TAG) {
            return dict_trie_.insert_user_word(word, freq, tag);
        }

        bool Find(const string &word) {
            return dict_trie_.find(word);
        }

        void reset_separators(const string &s) {
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

        void load_user_dict(const vector<string> &buf) {
            dict_trie_.LoadUserDict(buf);
        }

        void load_user_dict(const set<string> &buf) {
            dict_trie_.LoadUserDict(buf);
        }

        void load_user_dict(const string &path) {
            dict_trie_.LoadUserDict(path);
        }

    private:
        dict_trie dict_trie_;
        hmm_model model_;

        // They share the same dict trie and model
        mp_segment mp_seg_;
        HMMSegment hmm_seg_;
        mix_segment mix_seg_;
        full_segment full_seg_;
        query_segment query_seg_;

    public:
        keyword_extractor extractor;
    }; // class jieba_engine

} // namespace libnlp::jieba

