
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include <cassert>
#include "libnlp/dict/dict_base.h"
#include "libnlp/dict/lexicon.h"
#include "libnlp/dict/dict_entity.h"

namespace libnlp::dict {

    class dict_builder;

    class dict {
    public:
        dict(const dict &other) : _base_dict(other._base_dict) {
        }

        dict &operator=(const dict &other) {
            _base_dict = other._base_dict;
            return *this;
        }

        std::optional<const dict_entity *> match(const char *word,
                                                 size_t len) const {
            return _base_dict->match(word, len);
        }

        std::string type() {
            return _base_dict->type();
        }

        /**
         * Matches a word exactly and returns the dict_entity or std::nullopt.
         */
        dict_entity match(const std::string &word) const {
            auto r =  match(word.c_str(), word.length());
            if(r) {
                return dict_entity((*r)->key(), (*r)->values());
            }
            return dict_entity("");
        }

        /**
         * Matches the longest matched prefix of a word.
         * For example given a dictionary having "a", "an", "b", "ba", "ban", "bana",
         * the longest prefix of "banana" matched is "bana".
         */
        dict_entity match_prefix(const char *word, size_t len) const {
            assert(_base_dict);
            auto r =  _base_dict->match_prefix(word, len);
            if(r) {
                return dict_entity((*r)->key(), (*r)->values());
            }
            return dict_entity("");
        }

        /**
         * Matches the longest matched prefix of a word.
         */
        dict_entity match_prefix(const char *word) const {
            return match_prefix(word, key_max_length());
        }

        /**
         * Matches the longest matched prefix of a word.
         */
        dict_entity match_prefix(const std::string &word) const {
            return match_prefix(word.c_str(), word.length());
        }

        /**
         * Returns all matched prefixes of a word, sorted by the length (desc).
         * For example given a dictionary having "a", "an", "b", "ba", "ban", "bana",
         * all the matched prefixes of "banana" are "bana", "ban", "ba", "b".
         */
        std::vector<dict_entity> match_all_prefixes(const char *word,
                                                            size_t len) const {
            assert(_base_dict);
            auto rs =  _base_dict->match_all_prefixes(word, len);
            std::vector<dict_entity> ret;
            for(auto &r : rs) {
                ret.push_back(dict_entity(r->key(), r->values()));
            }
            return ret;
        }

        /**
         * Returns all matched prefixes of a word, sorted by the length (desc).
         */
        std::vector<dict_entity>
        match_all_prefixes(const std::string &word) const {
            return match_all_prefixes(word.c_str(), word.length());
        }

        /**
         * Returns the length of the longest key in the dictionary.
         */
        size_t key_max_length() const {
            return _base_dict->key_max_length();
        }

        /**
         * Returns all entries in the dictionary.
         */
        lexicon_ptr get_lexicon() const {
            return _base_dict->get_lexicon();
        }

        size_t size() const {
            return _base_dict->get_lexicon()->length();
        }

        dict_entity at(size_t index) const {
            auto r = _base_dict->get_lexicon()->at(index);
            return dict_entity(r->key(), r->values());
        }

        std::string to_string() {
            std::string ret = "format: " + _base_dict->type() + " size: " + std::to_string(size());
            return ret;
        }

        std::vector<std::unique_ptr<dict_entity>>::const_iterator begin() const {
            return get_lexicon()->begin();
        }

        std::vector<std::unique_ptr<dict_entity>>::const_iterator end() const {
            return get_lexicon()->end();
        }

        virtual ~dict() = default;

    private:

        friend class dict_builder;

        explicit dict(const basic_dict_ptr &ptr) : _base_dict(ptr) {
        }


        basic_dict_ptr _base_dict;
    };

    typedef std::shared_ptr<dict> dict_ptr;
}  // namespace libnlp::dict

