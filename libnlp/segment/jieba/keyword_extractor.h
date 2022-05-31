/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include <cmath>
#include <set>
#include "libnlp/segment/jieba/mix_segment.h"

namespace libnlp::jieba {

    using namespace libnlp;
    using namespace std;

    /*utf8*/
    class keyword_extractor {
    public:
        struct word_type {
            string word;
            vector<size_t> offsets;
            double weight;
        }; // struct word_type

        keyword_extractor(const string &dictPath,
                         const string &hmmFilePath,
                         const string &idfPath,
                         const string &stopWordPath,
                         const string &userDict = "")
                : segment_(dictPath, hmmFilePath, userDict) {
            load_idf_dict(idfPath);
            load_stop_word_dict(stopWordPath);
        }

        keyword_extractor(const dict_trie *dictTrie,
                         const hmm_model *model,
                         const string &idfPath,
                         const string &stopWordPath)
                : segment_(dictTrie, model) {
            load_idf_dict(idfPath);
            load_stop_word_dict(stopWordPath);
        }

        ~keyword_extractor() {
        }

        void extract(const string &sentence, vector<string> &keywords, size_t topN) const {
            vector<word_type> topWords;
            extract(sentence, topWords, topN);
            for (size_t i = 0; i < topWords.size(); i++) {
                keywords.push_back(topWords[i].word);
            }
        }

        void extract(const string &sentence, vector<pair<string, double> > &keywords, size_t topN) const {
            vector<word_type> topWords;
            extract(sentence, topWords, topN);
            for (size_t i = 0; i < topWords.size(); i++) {
                keywords.push_back(pair<string, double>(topWords[i].word, topWords[i].weight));
            }
        }

        void extract(const string &sentence, vector<word_type> &keywords, size_t topN) const {
            vector<string> words;
            segment_.cut(sentence, words);

            map<string, word_type> wordmap;
            size_t offset = 0;
            for (size_t i = 0; i < words.size(); ++i) {
                size_t t = offset;
                offset += words[i].size();
                if (is_single_word(words[i]) || stopWords_.find(words[i]) != stopWords_.end()) {
                    continue;
                }
                wordmap[words[i]].offsets.push_back(t);
                wordmap[words[i]].weight += 1.0;
            }
            if (offset != sentence.size()) {
                XLOG(ERROR) << "words illegal";
                return;
            }

            keywords.clear();
            keywords.reserve(wordmap.size());
            for (map<string, word_type>::iterator itr = wordmap.begin(); itr != wordmap.end(); ++itr) {
                unordered_map<string, double>::const_iterator cit = idfMap_.find(itr->first);
                if (cit != idfMap_.end()) {
                    itr->second.weight *= cit->second;
                } else {
                    itr->second.weight *= idfAverage_;
                }
                itr->second.word = itr->first;
                keywords.push_back(itr->second);
            }
            topN = min(topN, keywords.size());
            partial_sort(keywords.begin(), keywords.begin() + topN, keywords.end(), compare);
            keywords.resize(topN);
        }

    private:
        void load_idf_dict(const string &idfPath) {
            ifstream ifs(idfPath.c_str());
            XCHECK(ifs.is_open()) << "open " << idfPath << " failed";
            string line;
            vector<string> buf;
            double idf = 0.0;
            double idfSum = 0.0;
            size_t lineno = 0;
            for (; getline(ifs, line); lineno++) {
                buf.clear();
                if (line.empty()) {
                    XLOG(ERROR) << "lineno: " << lineno << " empty. skipped.";
                    continue;
                }
                Split(line, buf, " ");
                if (buf.size() != 2) {
                    XLOG(ERROR) << "line: " << line << ", lineno: " << lineno << " empty. skipped.";
                    continue;
                }
                idf = atof(buf[1].c_str());
                idfMap_[buf[0]] = idf;
                idfSum += idf;

            }

            assert(lineno);
            idfAverage_ = idfSum / lineno;
            assert(idfAverage_ > 0.0);
        }

        void load_stop_word_dict(const string &filePath) {
            ifstream ifs(filePath.c_str());
            XCHECK(ifs.is_open()) << "open " << filePath << " failed";
            string line;
            while (getline(ifs, line)) {
                stopWords_.insert(line);
            }
            assert(stopWords_.size());
        }

        static bool compare(const word_type &lhs, const word_type &rhs) {
            return lhs.weight > rhs.weight;
        }

        mix_segment segment_;
        unordered_map<string, double> idfMap_;
        double idfAverage_;

        unordered_set<string> stopWords_;
    }; // class keyword_extractor

    inline ostream &operator<<(ostream &os, const keyword_extractor::word_type &word) {
        return os << "{\"word\": \"" << word.word << "\", \"offset\": " << word.offsets << ", \"weight\": "
                  << word.weight << "}";
    }

} // namespace libnlp::jieba



