

/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include "libnlp/base/unicode.h"

namespace libnlp::jieba {

    using namespace std;

    const size_t MAX_WORD_LENGTH = 512;

    struct dict_unit {
        libnlp::unicode word;
        double weight;
        string tag;
    }; // struct dict_unit

    // for debugging
    // inline ostream & operator << (ostream& os, const dict_unit& unit) {
    //   string s;
    //   s << unit.word;
    //   return os << StringFormat("%s %s %.3lf", s.c_str(), unit.tag.c_str(), unit.weight);
    // }

    struct dag_entity {
        libnlp::rune_str runestr;
        // [offset, nexts.first]
        libnlp::inline_vector<pair<size_t, const dict_unit *>> nexts;
        const dict_unit *pInfo;
        double weight;
        size_t nextPos; // TODO
        dag_entity() : runestr(), pInfo(nullptr), weight(0.0), nextPos(0) {
        }
    }; // struct dag_entity

    typedef libnlp::rune_t TrieKey;

    class trie_node {
    public :
        trie_node() : next(nullptr), ptValue(nullptr) {
        }

    public:
        typedef unordered_map<TrieKey, trie_node *> NextMap;
        NextMap *next;
        const dict_unit *ptValue;
    };

    class base_trie {
    public:
        base_trie(const vector<libnlp::unicode> &keys, const vector<const dict_unit *> &valuePointers)
                : root_(new trie_node) {
            create_trie(keys, valuePointers);
        }

        ~base_trie() {
            delete_node(root_);
        }

        const dict_unit *
        find(libnlp::rune_str_array::const_iterator begin, libnlp::rune_str_array::const_iterator end) const {
            if (begin == end) {
                return nullptr;
            }

            const trie_node *ptNode = root_;
            trie_node::NextMap::const_iterator citer;
            for (libnlp::rune_str_array::const_iterator it = begin; it != end; it++) {
                if (nullptr == ptNode->next) {
                    return nullptr;
                }
                citer = ptNode->next->find(it->rune);
                if (ptNode->next->end() == citer) {
                    return nullptr;
                }
                ptNode = citer->second;
            }
            return ptNode->ptValue;
        }

        void find(libnlp::rune_str_array::const_iterator begin,
                  libnlp::rune_str_array::const_iterator end,
                  vector<struct dag_entity> &res,
                  size_t max_word_len = MAX_WORD_LENGTH) const {
            assert(root_ != nullptr);
            res.resize(end - begin);

            const trie_node *ptNode = nullptr;
            trie_node::NextMap::const_iterator citer;
            for (size_t i = 0; i < size_t(end - begin); i++) {
                res[i].runestr = *(begin + i);

                if (root_->next != nullptr && root_->next->end() != (citer = root_->next->find(res[i].runestr.rune))) {
                    ptNode = citer->second;
                } else {
                    ptNode = nullptr;
                }
                if (ptNode != nullptr) {
                    res[i].nexts.push_back(pair<size_t, const dict_unit *>(i, ptNode->ptValue));
                } else {
                    res[i].nexts.push_back(pair<size_t, const dict_unit *>(i, static_cast<const dict_unit *>(nullptr)));
                }

                for (size_t j = i + 1; j < size_t(end - begin) && (j - i + 1) <= max_word_len; j++) {
                    if (ptNode == nullptr || ptNode->next == nullptr) {
                        break;
                    }
                    citer = ptNode->next->find((begin + j)->rune);
                    if (ptNode->next->end() == citer) {
                        break;
                    }
                    ptNode = citer->second;
                    if (nullptr != ptNode->ptValue) {
                        res[i].nexts.push_back(pair<size_t, const dict_unit *>(j, ptNode->ptValue));
                    }
                }
            }
        }

        void insert_node(const libnlp::unicode &key, const dict_unit *ptValue) {
            if (key.begin() == key.end()) {
                return;
            }

            trie_node::NextMap::const_iterator kmIter;
            trie_node *ptNode = root_;
            for (libnlp::unicode::const_iterator citer = key.begin(); citer != key.end(); ++citer) {
                if (nullptr == ptNode->next) {
                    ptNode->next = new trie_node::NextMap;
                }
                kmIter = ptNode->next->find(*citer);
                if (ptNode->next->end() == kmIter) {
                    trie_node *nextNode = new trie_node;

                    ptNode->next->insert(make_pair(*citer, nextNode));
                    ptNode = nextNode;
                } else {
                    ptNode = kmIter->second;
                }
            }
            assert(ptNode != nullptr);
            ptNode->ptValue = ptValue;
        }

    private:
        void create_trie(const vector<libnlp::unicode> &keys, const vector<const dict_unit *> &valuePointers) {
            if (valuePointers.empty() || keys.empty()) {
                return;
            }
            assert(keys.size() == valuePointers.size());

            for (size_t i = 0; i < keys.size(); i++) {
                insert_node(keys[i], valuePointers[i]);
            }
        }

        void delete_node(trie_node *node) {
            if (nullptr == node) {
                return;
            }
            if (nullptr != node->next) {
                for (trie_node::NextMap::iterator it = node->next->begin(); it != node->next->end(); ++it) {
                    delete_node(it->second);
                }
                delete node->next;
            }
            delete node;
        }

        trie_node *root_;
    };
} // namespace libnlp::jieba

