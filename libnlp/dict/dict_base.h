
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#include "libnlp/base/common.h"
#include "libnlp/dict/dict_entity.h"

namespace libnlp::dict {
    /**
     * Abstract class of dictionary
     * @ingroup libnlp_dict
     */
    class LIBNLP_EXPORT basic_dict {
    public:
        /**
         * Matches a word exactly and returns the dict_entity or sstd::nullopt.
         */
        virtual std::optional<const dict_entity *> match(const char *word,
                                                  size_t len) const = 0;

        virtual std::string type() const = 0;

        /**
         * Matches a word exactly and returns the dict_entity or std::nullopt.
         */
        std::optional<const dict_entity *> match(const std::string &word) const {
            return match(word.c_str(), word.length());
        }

        /**
         * Matches the longest matched prefix of a word.
         * For example given a dictionary having "a", "an", "b", "ba", "ban", "bana",
         * the longest prefix of "banana" matched is "bana".
         */
        virtual std::optional<const dict_entity *> match_prefix(const char *word,
                                                        size_t len) const;

        /**
         * Matches the longest matched prefix of a word.
         */
        std::optional<const dict_entity *> match_prefix(const char *word) const {
            return match_prefix(word, key_max_length());
        }

        /**
         * Matches the longest matched prefix of a word.
         */
        std::optional<const dict_entity *> match_prefix(const std::string &word) const {
            return match_prefix(word.c_str(), word.length());
        }

        /**
         * Returns all matched prefixes of a word, sorted by the length (desc).
         * For example given a dictionary having "a", "an", "b", "ba", "ban", "bana",
         * all the matched prefixes of "banana" are "bana", "ban", "ba", "b".
         */
        virtual std::vector<const dict_entity *> match_all_prefixes(const char *word,
                                                                size_t len) const;

        /**
         * Returns all matched prefixes of a word, sorted by the length (desc).
         */
        std::vector<const dict_entity *>
        match_all_prefixes(const std::string &word) const {
            return match_all_prefixes(word.c_str(), word.length());
        }

        /**
         * Returns the length of the longest key in the dictionary.
         */
        virtual size_t key_max_length() const = 0;

        /**
         * Returns all entries in the dictionary.
         */
        virtual lexicon_ptr get_lexicon() const = 0;
    };
} // namespace libnlp::dict
