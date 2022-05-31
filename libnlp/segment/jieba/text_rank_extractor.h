

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <cmath>
#include "libnlp/segment/jieba/jieba.h"

namespace libnlp::jieba {
    using namespace libnlp;
    using namespace std;

    class text_rank_extractor {
    public:
        typedef struct _Word {
            string word;
            vector<size_t> offsets;
            double weight;
        } word_type; // struct word_type
    private:
        typedef std::map<string, word_type> word_map;

        class word_graph {
        private:
            typedef double Score;
            typedef string Node;
            typedef std::set<Node> node_set;

            typedef std::map<Node, double> Edges;
            typedef std::map<Node, Edges> Graph;
            //typedef std::unordered_map<Node,double> Edges;
            //typedef std::unordered_map<Node,Edges> Graph;

            double d;
            Graph graph;
            node_set nodeSet;
        public:
            word_graph() : d(0.85) {};

            word_graph(double in_d) : d(in_d) {};

            void addEdge(Node start, Node end, double weight) {
                Edges temp;
                Edges::iterator gotEdges;
                nodeSet.insert(start);
                nodeSet.insert(end);
                graph[start][end] += weight;
                graph[end][start] += weight;
            }

            void rank(word_map &ws, size_t rankTime = 10) {
                word_map outSum;
                Score wsdef, min_rank, max_rank;

                if (graph.size() == 0)
                    return;

                wsdef = 1.0 / graph.size();

                for (Graph::iterator edges = graph.begin(); edges != graph.end(); ++edges) {
                    // edges->first start节点；edge->first end节点；edge->second 权重
                    ws[edges->first].word = edges->first;
                    ws[edges->first].weight = wsdef;
                    outSum[edges->first].weight = 0;
                    for (Edges::iterator edge = edges->second.begin(); edge != edges->second.end(); ++edge) {
                        outSum[edges->first].weight += edge->second;
                    }
                }
                //sort(nodeSet.begin(),nodeSet.end()); 是否需要排序?
                for (size_t i = 0; i < rankTime; i++) {
                    for (node_set::iterator node = nodeSet.begin(); node != nodeSet.end(); node++) {
                        double s = 0;
                        for (Edges::iterator edge = graph[*node].begin(); edge != graph[*node].end(); edge++)
                            // edge->first end节点；edge->second 权重
                            s += edge->second / outSum[edge->first].weight * ws[edge->first].weight;
                        ws[*node].weight = (1 - d) + d * s;
                    }
                }

                min_rank = max_rank = ws.begin()->second.weight;
                for (word_map::iterator i = ws.begin(); i != ws.end(); i++) {
                    if (i->second.weight < min_rank) {
                        min_rank = i->second.weight;
                    }
                    if (i->second.weight > max_rank) {
                        max_rank = i->second.weight;
                    }
                }
                for (word_map::iterator i = ws.begin(); i != ws.end(); i++) {
                    ws[i->first].weight = (i->second.weight - min_rank / 10.0) / (max_rank - min_rank / 10.0);
                }
            }
        };

    public:
        text_rank_extractor(const string &dictPath,
                          const string &hmmFilePath,
                          const string &stopWordPath,
                          const string &userDict = "")
                : segment_(dictPath, hmmFilePath, userDict) {
            load_stop_word_dict(stopWordPath);
        }

        text_rank_extractor(const dict_trie *dictTrie,
                          const hmm_model *model,
                          const string &stopWordPath)
                : segment_(dictTrie, model) {
            load_stop_word_dict(stopWordPath);
        }

        text_rank_extractor(const jieba_engine &jieba, const string &stopWordPath) : segment_(jieba.get_dict_trie(),
                                                                                     jieba.get_hmm_model()) {
            load_stop_word_dict(stopWordPath);
        }

        ~text_rank_extractor() {
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

        void extract(const string &sentence, vector<word_type> &keywords, size_t topN, size_t span = 5,
                     size_t rankTime = 10) const {
            vector<string> words;
            segment_.cut(sentence, words);

            text_rank_extractor::word_graph graph;
            word_map wordmap;
            size_t offset = 0;

            for (size_t i = 0; i < words.size(); i++) {
                size_t t = offset;
                offset += words[i].size();
                if (is_single_word(words[i]) || stopWords_.find(words[i]) != stopWords_.end()) {
                    continue;
                }
                for (size_t j = i + 1, skip = 0; j < i + span + skip && j < words.size(); j++) {
                    if (is_single_word(words[j]) || stopWords_.find(words[j]) != stopWords_.end()) {
                        skip++;
                        continue;
                    }
                    graph.addEdge(words[i], words[j], 1);
                }
                wordmap[words[i]].offsets.push_back(t);
            }
            if (offset != sentence.size()) {
                XLOG(ERROR) << "words illegal";
                return;
            }

            graph.rank(wordmap, rankTime);

            keywords.clear();
            keywords.reserve(wordmap.size());
            for (word_map::iterator itr = wordmap.begin(); itr != wordmap.end(); ++itr) {
                keywords.push_back(itr->second);
            }

            topN = min(topN, keywords.size());
            partial_sort(keywords.begin(), keywords.begin() + topN, keywords.end(), Compare);
            keywords.resize(topN);
        }

    private:
        void load_stop_word_dict(const string &filePath) {
            ifstream ifs(filePath.c_str());
            XCHECK(ifs.is_open()) << "open " << filePath << " failed";
            string line;
            while (getline(ifs, line)) {
                stopWords_.insert(line);
            }
            assert(stopWords_.size());
        }

        static bool Compare(const word_type &x, const word_type &y) {
            return x.weight > y.weight;
        }

        mix_segment segment_;
        unordered_set<string> stopWords_;
    }; // class text_rank_extractor

    inline ostream &operator<<(ostream &os, const text_rank_extractor::word_type &word) {
        return os << "{\"word\": \"" << word.word << "\", \"offset\": " << word.offsets << ", \"weight\": "
                  << word.weight << "}";
    }
} // namespace libnlp::jieba


